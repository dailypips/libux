/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "queue.h"
#include "ux_internal.h"

void ux_barfactory_init(ux_barfactory_t* factory)
{
    factory->list_by_instrument_id = NULL;
}

void ux_barfactory_destory(ux_barfactory_t* factory)
{
}

void ux_barfactory_item_init(ux_barfactory_item_t *item)
{
    QUEUE_INIT(&item->queue_node);
}

void ux_barfactory_item_destory(ux_barfactory_item_t *item)
{

}


void ux_barfactory_add_item(ux_barfactory_t* factory, ux_barfactory_item_t* item)
{
    item_list_t* list;
    int instrument_id = item->instrument->id;

    HASH_FIND_INT(factory->list_by_instrument_id, &instrument_id, list);
    if (list == NULL) {
        list = ux_zalloc(sizeof(item_list_t));
        QUEUE_INIT(&list->queue);
        list->instrument_id = instrument_id;
        HASH_ADD_INT(factory->list_by_instrument_id, instrument_id, list);
    }
    item->factory = factory;
    QUEUE_INSERT_TAIL(&list->queue, &item->queue_node);
}

void ux_barfactory_remove_item(ux_barfactory_t* factory, ux_barfactory_item_t* item)
{
    UX_UNUSED(factory);
    QUEUE_REMOVE(&item->queue_node);
    item->factory = NULL;
}

static UX_AINLINE ux_loop_t*  get_loop(ux_barfactory_t *factory)
{
    UX_ASSERT(factory != NULL);
    return  container_of(factory, ux_loop_t, bar_factory);
}

void ux_barfactory_add_reminder(ux_barfactory_t *factory, ux_event_reminder_t *reminder)
{
    bus_add_timer(get_loop(factory), reminder);
}

static ux_event_tick_t* event_tick_new(ux_event_tick_t* e, ux_event_tick_t* n)
{
    //TODO: BID ?? TICK?? ASK??
    ux_event_tick_t* tick = (ux_event_tick_t*)ux_event_malloc(UX_EVENT_BID);
    tick->timestamp = e->timestamp;
    tick->instrument = e->instrument;
    tick->provider = e->provider;
    tick->price = (e->price + n->price) / 2.0;
    tick->size = (e->size + n->size) / 2;
    return tick;
}

void bar_factory_process_tick(ux_barfactory_t* factory, ux_event_tick_t* e)
{
    // find item_list by instrument_id
    // 在item_list上迭代，根据input类型
    // bar_factory_item_process_tick(item, e);
    item_list_t* list;
    QUEUE* q;
    ux_event_tick_t* tick;
    ux_loop_t* loop = get_loop(factory);

    int instrument_id = e->instrument;
    HASH_FIND_INT((item_list_t*)factory->list_by_instrument_id, &instrument_id, list);
    if (!list)
        return;

    QUEUE_FOREACH(q, &list->queue)
    {
        ux_barfactory_item_t* item = QUEUE_DATA(q, ux_barfactory_item_t, queue_node);
        switch (item->bar_input) {
        case UX_BAR_INPUT_TRADE:
            if (e->type == UX_EVENT_TRADE)
                item->on_tick(item, e);
            break;
        case UX_BAR_INPUT_BID:
            if (e->type == UX_EVENT_BID)
                item->on_tick(item, e);
            break;

        case UX_BAR_INPUT_ASK:
            if (e->type == UX_EVENT_ASK)
                item->on_tick(item, e);
            break;

        case UX_BAR_INPUT_MIDDLE:
            tick = NULL;
            if (e->type == UX_EVENT_TRADE)
                break;

            if (e->type == UX_EVENT_BID) {
                ux_event_ask_t* ask = data_manager_get_ask(&loop->data_manager, e->instrument);
                if (ask)
                    tick = event_tick_new(e, ask);
            }

            if (e->type == UX_EVENT_ASK) {
                ux_event_bid_t* bid = data_manager_get_bid(&loop->data_manager, e->instrument);
                if (bid)
                    tick = event_tick_new(e, bid);
            }

            item->on_tick(item, e);
            break;
        case UX_BAR_INPUT_TICK:
            item->on_tick(item, e);
            break;
        case UX_BAR_INPUT_BIDASK:
            if (e->type != UX_EVENT_TRADE)
                item->on_tick(item, e);
            break;
        }
    }
}

void ux_barfactory_emit_bar_open(ux_barfactory_t *factory, ux_barfactory_item_t *item)
{
    item->bar->status = UX_BAR_STATUS_OPEN;
    dispatch_event(get_loop(factory), (ux_event_t*)item->bar);
}

void ux_barfactory_emit_bar(ux_barfactory_t *factory, ux_barfactory_item_t *item)
{
    item->bar->status = UX_BAR_STATUS_CLOSE;
    dispatch_event(get_loop(factory), (ux_event_t*)item->bar);
    item->bar = NULL;
}

static void time_bar_on_reminder(ux_event_reminder_t *r)
{
    time_bar_item_t * item = (time_bar_item_t*)(r->user_data);
    if (item->clock_type == UX_CLOCK_LOCAL)
        item->bar->timestamp = r->stop;
    else
        item->bar->timestamp = bus_get_exchange_time(get_loop(item->factory));
    //TODO:
    // emit_bar();
}

void time_bar_on_tick(ux_barfactory_item_t *item, ux_event_tick_t *tick)
{
    int is_empty = item->bar == NULL;

    if (item->bar) {
        if(tick->price > item->bar->high)
            item->bar->high = tick->price;
        if (tick->price < item->bar->low)
            item->bar->low = tick->price;
        item->bar->close = tick->price;
        item->bar->volume += tick->size;
        //TODO:
        // item->bar.timestamp = ....
    } else {
        item->bar = (ux_event_bar_t*)ux_event_malloc(UX_EVENT_BAR);
        item->bar->provider = tick->provider;
        item->bar->instrument = tick->instrument;
        item->bar->bar_type = ((time_bar_item_t*)item)->bar_type;
        item->bar->size = ((time_bar_item_t*)item)->bar_size;
        //item->bar->open_time = item->
        //item->bar->close_time =
        item->bar->open = tick->price;
        item->bar->high = tick->price;
        item->bar->low  = tick->price;
        item->bar->close = tick->price;
        item->bar->volume = tick->size;
        //TODO:
        // emit_bar_open
    }
    item->bar->tick_count++;

    if(is_empty) {
        ux_event_reminder_t *r = (ux_event_reminder_t*)ux_event_malloc(UX_EVENT_REMINDER);
        r->clock_type = ((time_bar_item_t*)item)->clock_type;
        r->start = 0; //TODO
        r->stop = 0; // TODO
        r->repeat = 0; // TODO
        r->callback = time_bar_on_reminder;
        r->user_data = item;
        ux_barfactory_add_reminder(item->factory, r);
    }
}

void time_bar_item_init(time_bar_item_t *item)
{
    ux_barfactory_item_init((ux_barfactory_item_t*)item);
    item->clock_type = UX_CLOCK_LOCAL;
    item->on_tick = time_bar_on_tick;
}

void time_bar_item_destory(time_bar_item_t *item)
{
    ux_barfactory_item_destory((ux_barfactory_item_t*)item);
}
