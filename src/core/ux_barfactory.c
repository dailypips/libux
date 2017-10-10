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
    QUEUE_INIT(&item->queue_node);
    QUEUE_INSERT_TAIL(&list->queue, &item->queue_node);
}

void ux_barfactory_remove_item(ux_barfactory_t* factory, ux_barfactory_item_t* item)
{
    UX_UNUSED(factory);
    QUEUE_REMOVE(&item->queue_node);
    item->factory = NULL;
}

static UX_AINLINE ux_loop_t* get_loop(ux_barfactory_t* factory)
{
    UX_ASSERT(factory != NULL);
    return container_of(factory, ux_loop_t, bar_factory);
}

void ux_barfactory_add_reminder(ux_barfactory_t* factory, ux_event_reminder_t* reminder)
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

    int key = e->instrument;
    HASH_FIND_INT((item_list_t*)factory->list_by_instrument_id, &key, list);
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

void ux_barfactory_emit_bar_open(ux_barfactory_t* factory, ux_barfactory_item_t* item)
{
    item->bar->status = UX_BAR_STATUS_OPEN;
    ux_dispatch_event(get_loop(factory), (ux_event_t*)item->bar, UX_DISPATCH_IMMEDIATELY);
}

void ux_barfactory_emit_bar(ux_barfactory_t* factory, ux_barfactory_item_t* item)
{
    item->bar->status = UX_BAR_STATUS_CLOSE;
    ux_dispatch_event(get_loop(factory), (ux_event_t*)item->bar, UX_DISPATCH_IMMEDIATELY);
    item->bar = NULL;
}
