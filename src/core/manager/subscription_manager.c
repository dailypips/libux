/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "subscription_manager.h"
#include "context.h"
#include "instrument.h"

void subscription_manager_init(ux_ctx_t *ctx)
{
    ctx->subscriptions = kh_init(int);
}

void subscription_manager_destroy(ux_ctx_t *ctx)
{
    //TODO foreach clear
    kh_destroy(int, ctx->subscriptions);
}
