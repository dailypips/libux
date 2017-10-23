/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "dispatch.h"
#include "context.h"
#include "event_vtable.h"
#include <ux/domain/ux_provider.h>
#include "data_manager.h"
#include "bar_generator.h"

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
     uxe_bid_t *bid = (uxe_bid_t*)e;
     if (ctx->mode == UX_BUS_SIMULATION)
         bus_set_time(ctx, bid->timestamp);
     else
         bid->timestamp = bus_get_time(ctx);

     if(bid->exchange_timestamp > bus_get_exchange_time(ctx))
         bus_set_exchange_time(ctx, bid->exchange_timestamp);

     bar_factory_process_tick(ctx, (ux_event_tick_t*)e);
     ux_update_bid(ctx, bid);
     ctx->execution_simulator->on_bid(ctx->execution_simulator, bid);

     sm_on_bid(ctx, ux_get_instrument_by_id(ctx, bid->instrument), bid);

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
