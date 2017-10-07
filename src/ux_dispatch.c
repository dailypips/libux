#include "ux_internal.h"

void dispatch_event(ux_loop_t* loop, ux_event_t* e)
{
    // step 1: filter event

    // step 2: logger event

    // step 3: process event
    event_dispatch dispatcher = g_eventclassinfo[e->type].dispatch;
    if (dispatcher)
        dispatcher(loop, e);

    // step4: dispatch to other
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

    /*
     barFactory.OnTick(ask);
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
