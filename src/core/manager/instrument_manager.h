/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __INSTRUMENT_MANAGER_H__
#define __INSTRUMENT_MANAGER_H__

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INSTRUMENT_MANAGER_FIELDS                                              \
  ux_atomic_t next_instrument_id;                                              \
  ux_idarray_t instrument_by_id;                                               \
  ux_shash_t *instrument_by_symbol;                                            \
  void *instruments[2];

#define INSTRUMENT_MANAGER_INIT(ctx) instrument_manager_init((ctx))
#define INSTRUMENT_MANAGER_DESTROY(ctx) instrument_manager_destroy((ctx))

void instrument_manager_init(ux_ctx_t *ctx);
void instrument_manager_destroy(ux_ctx_t *ctx);

ux_instrument_t *ux_get_instrument_by_id(ux_ctx_t *ctx, int instrument_id);
ux_instrument_t *ux_get_instrument_by_symbol(ux_ctx_t *ctx, const char *symbol);
int ux_add_instrument(ux_ctx_t *ctx, ux_instrument_t *instrument);

#ifdef __cplusplus
}
#endif

#endif // __INSTRUMENT_MANAGER_H__
