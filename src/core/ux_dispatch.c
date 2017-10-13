#include "ux_internal.h"

static void on_event(ux_ctx_t *ctx, ux_event_t* e)
{
    // step 1: filter event

    // step 2: logger event

    // step 3: process event
    event_dispatch dispatcher = g_eventclassinfo[e->type].dispatch;
    if (dispatcher)
        dispatcher(ctx, e);

    // step4: dispatch to other

    // step5: unref
    ux_event_unref(e);
}

void ux_dispatch_event(ux_ctx_t *ctx, ux_event_t* e, ux_dispatch_mode mode)
{
    if (mode == UX_DISPATCH_IMMEDIATELY)
        on_event(ctx, e);
    else
        spscq_push(&ctx->buffed_event_queue, e);
}

void ux_emit_bufferd_events(ux_ctx_t *ctx)
{
    ux_event_t * e;
    while((e = spscq_pop(&ctx->buffed_event_queue))) {
        on_event(ctx, e);
    }
}

/* default process event */

void event_reminder_dispatch(ux_ctx_t *ctx, ux_event_t *e)
{

}

void event_ask_dispatch(ux_ctx_t *ctx, ux_event_t *e)
{
    printf("ask ");
}

void event_bid_dispatch(ux_ctx_t *ctx, ux_event_t *e)
{
     ctx->event_count++;
     ux_event_bid_t *bid = (ux_event_bid_t*)e;
     if (ctx->mode == UX_BUS_SIMULATION)
         bus_set_time(ctx, bid->timestamp);
     else
         bid->timestamp = bus_get_time(ctx);

     if(bid->exchange_timestamp > bus_get_exchange_time(ctx))
         bus_set_exchange_time(ctx, bid->exchange_timestamp);

     bar_factory_process_tick(&ctx->bar_factory, (ux_event_tick_t*)e);

     ctx->execution_simulator->on_bid(ctx->execution_simulator, bid);


    /*
     framework.dataManager.method_2(bid);
     framework.instrumentManager.GetById(bid.instrumentId).bid = bid;
     framework.providerManager.executionSimulator.OnBid(bid);
     framework.strategyManager.method_12(bid);
     framework.strategyManager__0.method_8(bid);
     */
}

void event_trade_dispatch(ux_ctx_t *ctx, ux_event_t *e)
{
     printf("trade ");
}
