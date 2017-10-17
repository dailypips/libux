/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "queue.h"
#include "ux_internal.h"

static void* get_key_value(khash_t(int) *hash, int key)
{
    khint_t iter = kh_get(int, hash, key);
    if (iter == kh_end(hash))
        return NULL;
    return &kh_value(hash, iter);
}

void ux_barfactory_init(ux_ctx_t *ctx)
{
    ctx->list_by_instrument_id = NULL;
}

void ux_barfactory_destory(ux_ctx_t *ctx)
{
}

void ux_barfactory_add_item(ux_ctx_t *ctx, ux_barfactory_item_t* item)
{
    _list_t* list;
    khint_t iter;
    int ret;

    iter = kh_get(list, ctx->list_by_instrument_id, item->instrument->id);
    if (iter == kh_end(ctx->list_by_instrument_id)) { /* not found */
        iter = kh_put(list, ctx->list_by_instrument_id, item->instrument->id, &ret);
        UX_ASSERT(ret > 0);
        list = &(kh_value(ctx->list_by_instrument_id, iter));
        QUEUE_INIT(&list->queue);
    }
    item->ctx = ctx;
    QUEUE_INSERT_TAIL(&list->queue, &item->queue_node);
}

void ux_barfactory_remove_item(ux_ctx_t *ctx, ux_barfactory_item_t* item)
{
    UX_UNUSED(ctx);
    QUEUE_REMOVE(&item->queue_node);
    item->ctx = NULL;
}

void ux_barfactory_add_reminder(ux_ctx_t *ctx, uxe_reminder_t* reminder)
{
    bus_add_timer(ctx, reminder);
}

static ux_event_tick_t* event_tick_new(ux_event_tick_t* e, ux_event_tick_t* n)
{
    //TODO: BID ?? TICK?? ASK??
    ux_event_tick_t* tick = (ux_event_tick_t*)ux_event_malloc(UXE_BID);
    tick->timestamp = e->timestamp;
    tick->instrument = e->instrument;
    tick->provider = e->provider;
    tick->price = (e->price + n->price) / 2.0;
    tick->size = (e->size + n->size) / 2;
    return tick;
}

void bar_factory_process_tick(ux_ctx_t *ctx, ux_event_tick_t* e)
{
    // find item_list by instrument_id
    // 在item_list上迭代，根据input类型
    // bar_factory_item_process_tick(item, e);
    _list_t* list;
    QUEUE* q;
    ux_event_tick_t* tick;
    khint_t iter;

    iter = kh_get(list, ctx->list_by_instrument_id, e->instrument);
    if (iter == kh_end(ctx->list_by_instrument_id)) /* not found */
        return;

    list = &(kh_value(ctx->list_by_instrument_id, iter));

    QUEUE_FOREACH(q, &list->queue)
    {
        ux_barfactory_item_t* item = QUEUE_DATA(q, ux_barfactory_item_t, queue_node);

        switch (item->bar_input) {

        case UX_BAR_INPUT_TRADE:
            if (e->type == UXE_TRADE)
                item->on_tick(item, e);
            break;

        case UX_BAR_INPUT_BID:
            if (e->type == UXE_BID)
                item->on_tick(item, e);
            break;

        case UX_BAR_INPUT_ASK:
            if (e->type == UXE_ASK)
                item->on_tick(item, e);
            break;

        case UX_BAR_INPUT_MIDDLE:
            tick = NULL;
            if (e->type == UXE_TRADE)
                break;

            if (e->type == UXE_BID) {
                uxe_ask_t* ask = data_manager_get_ask(ctx, e->instrument);
                if (ask)
                    tick = event_tick_new(e, (ux_event_tick_t*)ask);
            }

            if (e->type == UXE_ASK) {
                uxe_bid_t* bid = data_manager_get_bid(ctx, e->instrument);
                if (bid)
                    tick = event_tick_new(e, (ux_event_tick_t*)bid);
            }

            item->on_tick(item, e);
            break;
        case UX_BAR_INPUT_TICK:
            item->on_tick(item, e);
            break;
        case UX_BAR_INPUT_BIDASK:
            if (e->type != UXE_TRADE)
                item->on_tick(item, e);
            break;
        }
    }
}

void ux_barfactory_emit_bar_open(ux_ctx_t *ctx, ux_barfactory_item_t* item)
{
    item->bar->status = UX_BAR_STATUS_OPEN;
    ux_dispatch_event(ctx, (ux_event_t*)item->bar, UX_DISPATCH_IMMEDIATELY);
}

void ux_barfactory_emit_bar(ux_ctx_t *ctx, ux_barfactory_item_t* item)
{
    item->bar->status = UX_BAR_STATUS_CLOSE;
    ux_dispatch_event(ctx, (ux_event_t*)item->bar, UX_DISPATCH_IMMEDIATELY);
    item->bar = NULL;
}
