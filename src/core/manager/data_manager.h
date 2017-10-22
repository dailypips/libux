#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "context.h"

#define DATA_MANAGER_FIELDS                                                    \
  ux_idarray_t dm_asks;                                                        \
  ux_idarray_t dm_bids;                                                        \
  ux_idarray_t dm_trades;

uxe_ask_t *data_manager_get_ask(ux_ctx_t *ctx, int instrument_id);
uxe_bid_t *data_manager_get_bid(ux_ctx_t *ctx, int instrument_id);
uxe_trade_t *data_manager_get_trade(ux_ctx_t *ctx, int instrument_id);
void data_manager_set_ask(ux_ctx_t *ctx, uxe_ask_t *ask);
void data_manager_set_bid(ux_ctx_t *ctx, uxe_bid_t *bid);
void data_manager_set_trade(ux_ctx_t *ctx, uxe_trade_t *trade);


#endif // __DATA_MANAGER_H__
