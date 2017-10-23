/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __ORDER_MANAGER_H__
#define __ORDER_MANAGER_H__

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ORDER_MANAGER_FIELDS                                                   \
  ux_atomic_t next_order_id;                                                   \
  ux_idarray_t order_by_id;                                                    \
  ux_shash_t *order_by_client_oid;                                             \
  ux_shash_t *order_by_provider_oid;                                           \
  ux_list_shash_t *order_list_by_oca;                                          \
  void *orders[2];

#define ORDER_MANAGER_INIT(ctx) order_manager_init((ctx))
#define ORDER_MANAGER_DESTROY(ctx) order_manager_destroy((ctx))

#define UX_ORDER_HAS_REGISTERED -1
#define UX_ORDER_SENT           -2

UX_FUNC void order_manager_init(ux_ctx_t *ctx);
UX_FUNC void order_manager_destroy(ux_ctx_t *ctx);

ux_order_t *ux_get_order_by_id(ux_ctx_t *ctx, int id);
void ux_add_order_by_id(ux_ctx_t *ctx, int id, ux_order_t *order);
ux_order_t *ux_get_order_by_client_oid(ux_ctx_t *ctx, const char *oid);
void ux_add_order_by_client_oid(ux_ctx_t *ctx, const char *oid,
                                ux_order_t *order);
ux_order_t *ux_get_order_by_provider_oid(ux_ctx_t *ctx, const char *oid);
void ux_add_order_by_oca(ux_ctx_t *ctx, ux_order_t *order);
ux_list_t *ux_get_order_list_by_oca(ux_ctx_t *ctx, const char *oca);
int ux_add_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_register_order(ux_ctx_t *ctx, ux_order_t *order);
void ux_reject_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_send_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_cancel_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_replace_order(ux_ctx_t *ctx, ux_order_t *order, double price,
                     double stop_price, double qty);

#ifdef __cplusplus
}
#endif

#endif // __ORDER_MANAGER_H__
