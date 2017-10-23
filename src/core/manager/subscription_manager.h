/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef SUBSCRIPTION_MANAGER_H
#define SUBSCRIPTION_MANAGER_H

#include <ux/base/ux_type.h>
#include "hash.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct subscription_manager_s subscription_manager_t;

#define SUBSCRIPTION_MANANGER_FIELDS \
    int connect_on_subscribe; \
    ux_ihash_t *subscriptions;

// subscriptions <int, Dictionary<Instrument, int>>
struct subscription_manager_s {
    SUBSCRIPTION_MANANGER_FIELDS
};

#define SUBSCRIPTION_MANANGER_INIT(ctx)   subscription_manager_init((ctx))
#define SUBSCRIPTION_MANANGER_DESTROY(ctx) subscription_manager_destroy((ctx))

void subscription_manager_init(ux_ctx_t *ctx);
void subscription_manager_destroy(ux_ctx_t *ctx);


void subscription_manager_subscribe(ux_ctx_t *ctx,
                                    ux_data_provider_t *provider,
                                    ux_instrument_t *instrument);

void subscription_manager_unsubscribe(ux_ctx_t *ctx,
                                    ux_data_provider_t *provider,
                                    ux_instrument_t *instrument);

int subscription_manager_is_subscribed(ux_ctx_t *ctx,
                                       ux_data_provider_t *provider,
                                       ux_instrument_t *instrument);

void subscription_manager_resubscribe(ux_ctx_t *ctx,
                                      ux_data_provider_t *provider);

void subscription_manager_desubscribe(ux_ctx_t *ctx,
                                      ux_data_provider_t *provider);

void subscription_manager_clear(ux_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // SUBSCRIPTION_MANAGER_H
