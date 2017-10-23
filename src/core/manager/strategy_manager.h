/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __STRATEGY_MANAGER_H__
#define __STRATEGY_MANAGER_H__

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STRATEGY_MANAGER_FIELDS

#define STRATEGY_MANAGER_INIT(ctx) strategy_manager_init(ctx)
#define STRATEGY_MANAGER_DESTROY(ctx) strategy_manager_destroy(ctx)

UX_FUNC void strategy_manager_init(ux_ctx_t *ctx);
UX_FUNC void strategy_manager_destroy(ux_ctx_t *ctx);

ux_strategy_t *ux_get_strategy_by_id(ux_ctx_t *ctx, int id);
void ux_add_strategy_by_id(ux_ctx_t *ctx, int id, ux_strategy_t *portfolio);


#ifdef __cplusplus
}
#endif

#endif // __STRATEGY_MANAGER_H__
