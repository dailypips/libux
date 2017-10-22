/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__


ux_instrument_t *ux_get_instrument_by_id(ux_ctx_t *ctx, int instrument_id);
ux_instrument_t *ux_get_instrument_by_symbol(ux_ctx_t *ctx, const char *symbol);

#define UX_ORDER_RESULT_OK       0
#define UX_ORDER_HAS_REGISTERED -1
#define UX_ORDER_SENT           -2
int ux_register_order(ux_ctx_t *ctx, ux_order_t *order);
void ux_reject_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_send_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_cancel_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_replace_order(ux_ctx_t *ctx, ux_order_t *order, double price, double stop_price, double qty);


#endif // __INSTRUMENT_H__
