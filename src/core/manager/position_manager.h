/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __POSITION_MANAGER_H__
#define __POSITION_MANAGER_H__

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define POSITION_MANAGER_FIELDS

#define POSITION_MANAGER_INIT(ctx) position_manager_init(ctx)
#define POSITION_MANAGER_DESTROY(ctx) position_manager_destroy(ctx)

UX_FUNC void position_manager_init(ux_ctx_t *ctx);
UX_FUNC void position_manager_destroy(ux_ctx_t *ctx);

ux_position_t *ux_get_position_by_id(ux_ctx_t *ctx, int id);
void ux_add_position_by_id(ux_ctx_t *ctx, int id, ux_position_t *position);


#ifdef __cplusplus
}
#endif

#endif // __POSITION_MANAGER_H__
