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
        ux_event_unref((ux_event_t*)(node->e));
    }

    ux_event_ref((ux_event_t*)tick);

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

void data_manager_set_ask(data_manager_t* manager, ux_event_ask_t* ask)
{
    set_tick(&manager->ask_hash, ask);
}

void data_manager_set_bid(data_manager_t* manager, ux_event_bid_t* bid)
{
    set_tick(&manager->bid_hash, bid);
}

void data_manager_set_trade(data_manager_t* manager, ux_event_trade_t* trade)
{
    set_tick(&manager->trade_hash, trade);
}

ux_event_ask_t* data_manager_get_ask(data_manager_t* manager, int instrument_id)
{
    return get_key_value(&manager->ask_hash, instrument_id);
}

ux_event_bid_t* data_manager_get_bid(data_manager_t* manager, int instrument_id)
{
    return get_key_value(&manager->bid_hash, instrument_id);
}

ux_event_trade_t* data_manager_get_trade(data_manager_t* manager, int instrument_id)
{
    return get_key_value(&manager->trade_hash, instrument_id);
}

/*ux_event_ask_t* data_manager_get_ask_with_provider(data_manager_t* manager, int instrument_id, int provider_id)
{
    UX_ASSERT(instrument_id >= UX_INSTRUMENT_ID_MIN && instrument_id <= UX_INSTRUMENT_ID_MAX);
    UX_ASSERT(provider_id >= UX_PROVIDER_ID_MIN && provider_id <= UX_PROVIDRE_ID_MAX);
    return get_key_value(&manager->ask_hash, instrument_id * provider_id);
}

ux_event_ask_t* data_manager_get_bid_with_provider(data_manager_t* manager, int instrument_id, int provider_id)
{
    UX_ASSERT(instrument_id >= UX_INSTRUMENT_ID_MIN && instrument_id <= UX_INSTRUMENT_ID_MAX);
    UX_ASSERT(provider_id >= UX_PROVIDER_ID_MIN && provider_id <= UX_PROVIDRE_ID_MAX);
    return get_key_value(&manager->bid_hash, instrument_id * provider_id);
}

ux_event_ask_t* data_manager_get_trade_with_provider(data_manager_t* manager, int instrument_id, int provider_id)
{
    UX_ASSERT(instrument_id >= UX_INSTRUMENT_ID_MIN && instrument_id <= UX_INSTRUMENT_ID_MAX);
    UX_ASSERT(provider_id >= UX_PROVIDER_ID_MIN && provider_id <= UX_PROVIDRE_ID_MAX);
    return get_key_value(&manager->trade_hash, instrument_id * provider_id);
}
*/

void data_manager_init(data_manager_t *manager)
{
    manager->ask_hash = NULL;
    manager->bid_hash = NULL;
    manager->trade_hash = NULL;

}


static void hash_delete_all(tick_hash_node_t **hash)
{
    tick_hash_node_t *current_node, *tmp;

    HASH_ITER(hh, *hash, current_node, tmp) {
      HASH_DEL(*hash,current_node);
      ux_free(current_node);
    }
}

void data_manager_destory(data_manager_t *manager)
{
    hash_delete_all(&manager->ask_hash);
    hash_delete_all(&manager->bid_hash);
    hash_delete_all(&manager->trade_hash);
}
