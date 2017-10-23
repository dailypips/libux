/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "ux_internal.h"

static void set_key_value(ux_ihash_t *hash, int key, ux_event_tick_t* tick)
{
    int ret;
    khint_t k;

    k = kh_put(int, hash, key, &ret);

    if(ret == 0) // key exist
        ux_event_unref((ux_event_t*)kh_value(hash,k));

    kh_value(hash, k) = tick;

    ux_event_ref((ux_event_t*)tick);
}

static ux_event_tick_t* get_key_value(ux_ihash_t *hash, int key)
{
    khint_t iter = kh_get(int, hash, key);
    if (iter == kh_end(hash))
        return NULL;
    return kh_value(hash, iter);
}

static inline void set_tick(ux_ihash_t *hash, ux_event_tick_t* tick)
{
    set_key_value(hash, tick->instrument, tick);
}

void dm_set_ask(ux_ctx_t *ctx, uxe_ask_t* ask)
{
    set_tick(ctx->dm_asks, (ux_event_tick_t*)ask);
}

void dm_set_bid(ux_ctx_t *ctx, uxe_bid_t* bid)
{
    set_tick(ctx->dm_bids, (ux_event_tick_t*)bid);
}

void dm_set_trade(ux_ctx_t *ctx, uxe_trade_t* trade)
{
    set_tick(ctx->dm_trades, (ux_event_tick_t*)trade);
}

uxe_ask_t* dm_get_ask(ux_ctx_t *ctx, int instrument_id)
{
   return (uxe_ask_t*)get_key_value(ctx->dm_asks, instrument_id);
}

uxe_bid_t* dm_get_bid(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_bid_t*)get_key_value(ctx->dm_bids, instrument_id);
}

uxe_trade_t* dm_get_trade(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_trade_t*)get_key_value(ctx->dm_trades, instrument_id);
}

void dm_init(ux_ctx_t *ctx)
{
    ctx->dm_asks = kh_init(int);
    ctx->dm_bids = kh_init(int);
    ctx->dm_trades = kh_init(int);
}


static void hash_delete_all(ux_ihash_t *hashtable)
{
    ux_event_t *val;
    kh_foreach_value(hashtable, val, ux_event_unref(val));
    kh_destroy(int, hashtable);
}

void dm_destroy(ux_ctx_t *ctx)
{
    hash_delete_all(ctx->dm_asks);
    hash_delete_all(ctx->dm_bids);
    hash_delete_all(ctx->dm_trades);
}

//void dm_add_data_series(ux_instrument_t *instrument, uxe_type type, ux_bar_type bar_type, long bar_szie);
//ux_data_series_t* dm_get_data_series(ux_instrument_t *instrument, uxe_type type, ux_bar_type bar_type, long bar_size);

