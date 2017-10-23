/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "context.h"

#ifdef __cplusplus
extern  "C" {
#endif


#define DATA_MANAGER_FIELDS                                                    \
  ux_idarray_t dm_asks;                                                        \
  ux_idarray_t dm_bids;                                                        \
  ux_idarray_t dm_trades;

#define DATA_MANAGER_INIT(ctx)    data_manager_init((ctx))
#define DATA_MANAGER_DESTROY(ctx) data_manager_destroy((ctx))

uxe_ask_t *ux_get_last_ask(ux_ctx_t *ctx, int instrument_id);
uxe_bid_t *ux_get_last_bid(ux_ctx_t *ctx, int instrument_id);
uxe_trade_t *ux_get_last_trade(ux_ctx_t *ctx, int instrument_id);
void ux_update_ask(ux_ctx_t *ctx, uxe_ask_t *ask);
void ux_update_bid(ux_ctx_t *ctx, uxe_bid_t *bid);
void ux_update_trade(ux_ctx_t *ctx, uxe_trade_t *trade);

void data_manager_init(ux_ctx_t *ctx);
void data_manager_destroy(ux_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // __DATA_MANAGER_H__
