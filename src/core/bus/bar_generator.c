/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "queue.h"
#include "bar_generator.h"
#include "context.h"
#include <string.h>
#include "hash.h"
#include "dispatch.h"
#include <ux/event/tick.h>
#include "instrument.h"
#include "data_manager.h"

void ux_bar_factory_init(ux_ctx_t *ctx)
{
    ctx->list_by_instrument_id = NULL;
}

void ux_bar_factory_destory(ux_ctx_t *ctx)
{
}

void ux_bar_factory_add_generator(ux_ctx_t *ctx, ux_bar_generator_t* generator)
{
    ux_list_t* list = int_hash_get_or_create_list(ctx->list_by_instrument_id, generator->instrument->id);
    generator->ctx = ctx;
    QUEUE_INSERT_TAIL(&list->queue, &generator->queue_node);
}

void ux_bar_factory_remove_generator(ux_ctx_t *ctx, ux_bar_generator_t* generator)
{
    UX_UNUSED(ctx);
    QUEUE_REMOVE(&generator->queue_node);
    generator->ctx = NULL;
}

void ux_bar_factory_add_reminder(ux_ctx_t *ctx, uxe_reminder_t* reminder)
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
    QUEUE* q;
    ux_event_tick_t* tick;

    ux_list_t *list = int_hash_get_list(ctx->list_by_instrument_id, e->instrument);
    if(!list)
        return;

    QUEUE_FOREACH(q, &list->queue)
    {
        ux_bar_generator_t* item = QUEUE_DATA(q, ux_bar_generator_t, queue_node);

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

void ux_bar_factory_emit_bar_open(ux_ctx_t *ctx, ux_bar_generator_t* item)
{
    item->bar->status = UX_BAR_STATUS_OPEN;
    ux_dispatch_event(ctx, (ux_event_t*)item->bar, UX_DISPATCH_IMMEDIATELY);
}

void ux_bar_factory_emit_bar(ux_ctx_t *ctx, ux_bar_generator_t* item)
{
    item->bar->status = UX_BAR_STATUS_CLOSE;
    ux_dispatch_event(ctx, (ux_event_t*)item->bar, UX_DISPATCH_IMMEDIATELY);
    item->bar = NULL;
}
