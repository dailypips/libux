#include "ux_internal.h"

static void on_event(ux_loop_t* loop, ux_event_t* e)
{
    // step 1: filter event

    // step 2: logger event

    // step 3: process event
    event_dispatch dispatcher = g_eventclassinfo[e->type].dispatch;
    if (dispatcher)
        dispatcher(loop, e);

    // step4: dispatch to other

    // step5: unref
    ux_event_unref(e);
}

void ux_dispatch_event(ux_loop_t* loop, ux_event_t* e, ux_dispatch_mode mode)
{
    if (mode == UX_DISPATCH_IMMEDIATELY)
        on_event(loop, e);
    else
        spscq_push(&loop->buffed_event_queue, e);
}

void ux_emit_bufferd_events(ux_loop_t *loop)
{
    ux_event_t * e;
    while((e = spscq_pop(&loop->buffed_event_queue))) {
        on_event(loop, e);
    }
}

/* default process event */

void event_reminder_dispatch(ux_loop_t *loop, ux_event_t *e)
{

}

void event_ask_dispatch(ux_loop_t *loop, ux_event_t *e)
{
    printf("ask ");
}

void event_bid_dispatch(ux_loop_t *loop, ux_event_t *e)
{
     loop->event_count++;
     ux_event_bid_t *bid = (ux_event_bid_t*)e;
     if (loop->mode == UX_BUS_SIMULATION)
         bus_set_time(loop, bid->timestamp);
     else
         bid->timestamp = bus_get_time(loop);

     if(bid->exchange_timestamp > bus_get_exchange_time(loop))
         bus_set_exchange_time(loop, bid->exchange_timestamp);

     bar_factory_process_tick(&loop->bar_factory, (ux_event_tick_t*)e);

    /*
     framework.dataManager.method_2(ask);
     framework.instrumentManager.GetById(ask.instrumentId).ask = ask;
     framework.providerManager.executionSimulator.OnAsk(ask);
     framework.strategyManager.method_12(ask);
     framework.strategyManager__0.method_8(ask);
     */
}

void event_trade_dispatch(ux_loop_t *loop, ux_event_t *e)
{
     printf("trade ");
}
