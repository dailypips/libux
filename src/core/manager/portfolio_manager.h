/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __PORTFOLIO_MANAGER_H__
#define __PORTFOLIO_MANAGER_H__

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PORTFOLIO_MANAGER_FIELDS

#define PORTFOLIO_MANAGER_INIT(ctx) portfolio_manager_init(ctx)
#define PORTFOLIO_MANAGER_DESTROY(ctx) portfolio_manager_destroy(ctx)

UX_FUNC void portfolio_manager_init(ux_ctx_t *ctx);
UX_FUNC void portfolio_manager_destroy(ux_ctx_t *ctx);

ux_portfolio_t *ux_get_portfolio_by_id(ux_ctx_t *ctx, int id);
void ux_add_portfolio_by_id(ux_ctx_t *ctx, int id, ux_portfolio_t *portfolio);


#ifdef __cplusplus
}
#endif

#endif // __PORTFOLIO_MANAGER_H__
