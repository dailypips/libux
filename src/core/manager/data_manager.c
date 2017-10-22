/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "data_manager.h"

static void set_key_value(ux_idarray_t *v, int key, ux_event_tick_t* tick)
{
    uintptr_t k = kv_get(v, key);

    if(k) // key exist
        ux_event_unref((ux_event_t*)k);

    kv_set(v, key, (uintptr_t)tick);

    ux_event_ref((ux_event_t*)tick);
}

static inline void set_tick(ux_idarray_t *v, ux_event_tick_t* tick)
{
    set_key_value(v, tick->instrument, tick);
}

void dm_set_ask(ux_ctx_t *ctx, uxe_ask_t* ask)
{
    set_tick(&ctx->dm_asks, (ux_event_tick_t*)ask);
}

void dm_set_bid(ux_ctx_t *ctx, uxe_bid_t* bid)
{
    set_tick(&ctx->dm_bids, (ux_event_tick_t*)bid);
}

void dm_set_trade(ux_ctx_t *ctx, uxe_trade_t* trade)
{
    set_tick(&ctx->dm_trades, (ux_event_tick_t*)trade);
}

uxe_ask_t* dm_get_ask(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_ask_t*)kv_get(&ctx->dm_asks, instrument_id);
}

uxe_bid_t* dm_get_bid(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_bid_t*)kv_get(&ctx->dm_asks, instrument_id);
}

uxe_trade_t* dm_get_trade(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_trade_t*)kv_get(&ctx->dm_trades, instrument_id);
}

void dm_init(ux_ctx_t *ctx)
{
    kv_init(&ctx->dm_asks);
    kv_init(&ctx->dm_bids);
    kv_init(&ctx->dm_trades);
}

static inline void delete_all(ux_idarray_t *v)
{
    for(size_t i = 0; i < v->n; i++)
        ux_event_unref((ux_event_t*)kv_get(v, i));
    kv_destroy(v);
}
void dm_destroy(ux_ctx_t *ctx)
{
    delete_all(&ctx->dm_asks);
    delete_all(&ctx->dm_bids);
    delete_all(&ctx->dm_trades);
}

//void dm_add_data_series(ux_instrument_t *instrument, uxe_type type, ux_bar_type bar_type, long bar_szie);
//ux_data_series_t* dm_get_data_series(ux_instrument_t *instrument, uxe_type type, ux_bar_type bar_type, long bar_size);

