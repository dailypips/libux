/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

static void set_key_value(tick_hash_node_t** hash, int key, ux_event_tick_t* tick)
{
    tick_hash_node_t* node;

    HASH_FIND_INT(*hash, &key, node);

    if (node == NULL) {
        node = ux_zalloc(sizeof(tick_hash_node_t));
        HASH_ADD_INT(*hash, key, node);
    } else {
        uxe_unref((ux_event_t*)(node->e));
    }

    uxe_ref((ux_event_t*)tick);

    node->e = tick;
}

static ux_event_tick_t* get_key_value(tick_hash_node_t** hash, int key)
{
    tick_hash_node_t* node;

    HASH_FIND_INT(*hash, &key, node);
    if (node != NULL) {
        return node->e;
    }

    return NULL;
}

static void set_tick(tick_hash_node_t** hash, ux_event_tick_t* tick)
{
    int key = tick->instrument;

    set_key_value(hash, key, tick);
}

void data_manager_set_ask(ux_ctx_t *ctx, uxe_ask_t* ask)
{
    set_tick(&ctx->dm_ask_hash, (ux_event_tick_t*)ask);
}

void data_manager_set_bid(ux_ctx_t *ctx, uxe_bid_t* bid)
{
    set_tick(&ctx->dm_bid_hash, (ux_event_tick_t*)bid);
}

void data_manager_set_trade(ux_ctx_t *ctx, uxe_trade_t* trade)
{
    set_tick(&ctx->dm_trade_hash, (ux_event_tick_t*)trade);
}

uxe_ask_t* data_manager_get_ask(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_ask_t*)get_key_value(&ctx->dm_ask_hash, instrument_id);
}

uxe_bid_t* data_manager_get_bid(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_bid_t*)get_key_value(&ctx->dm_bid_hash, instrument_id);
}

uxe_trade_t* data_manager_get_trade(ux_ctx_t *ctx, int instrument_id)
{
    return (uxe_trade_t*)get_key_value(&ctx->dm_trade_hash, instrument_id);
}

void data_manager_init(ux_ctx_t *ctx)
{
    ctx->dm_ask_hash = NULL;
    ctx->dm_bid_hash = NULL;
    ctx->dm_trade_hash = NULL;
}


static void hash_delete_all(tick_hash_node_t **hash)
{
    tick_hash_node_t *current_node, *tmp;

    HASH_ITER(hh, *hash, current_node, tmp) {
      HASH_DEL(*hash,current_node);
      ux_free(current_node);
    }
}

void data_manager_destory(ux_ctx_t *ctx)
{
    hash_delete_all(&ctx->dm_ask_hash);
    hash_delete_all(&ctx->dm_bid_hash);
    hash_delete_all(&ctx->dm_trade_hash);
}
