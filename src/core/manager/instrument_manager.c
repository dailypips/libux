/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "instrument_manager.h"
#include "context.h"
#include "hash.h"
#include "queue.h"

// TODO: instrument manager init
/* instrument manager */
void instrument_manager_init(ux_ctx_t *ctx) {
  ida_int(&ctx->instrument_by_id);
  ctx->instrument_by_symbol = kh_init(str);
  ctx->next_instrument_id = 0;
}

void instrument_manager_destroy(ux_ctx_t *ctx) {
  // instrument_by_id nothing to do
  // instrument_by_symbol remove hashtable
  kh_destroy(str, ctx->instrument_by_symbol);
}

ux_instrument_t *ux_get_instrument_by_id(ux_ctx_t *ctx, int id) {
  return (ux_instrument_t *)ida_get(&ctx->instrument_by_id, id);
}

ux_instrument_t *ux_get_instrument_by_symbol(ux_ctx_t *ctx,
                                             const char *symbol) {
  khint_t iter = kh_get(str, ctx->instrument_by_symbol, symbol);
  if (iter == kh_end(ctx->instrument_by_symbol))
    return NULL;
  return kh_value(ctx->instrument_by_symbol, iter);
}

int ux_add_instrument(ux_ctx_t *ctx, ux_instrument_t *instrument) {
  int ret;
  ux_instrument_t *inst = ux_get_instrument_by_symbol(ctx, instrument->symbol);
  if (inst)
    return -1;
  instrument->id = ctx->next_instrument_id++;
  ida_set(&ctx->instrument_by_id, instrument->id, (uintptr_t)instrument);
  khint_t iter =
      kh_put_str(ctx->instrument_by_symbol, instrument->symbol, &ret);
  assert(ret == 0);
  kh_value(ctx->instrument_by_symbol, iter) = instrument;
  return 0;
}
