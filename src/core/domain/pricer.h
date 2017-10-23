/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef PRICER_H
#define PRICER_H

#include <ux/base/ux_type.h>
#include <ux/domain/ux_portfolio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct default_pricer_s {
    UX_PRICER_PUBLIC_FIELDS
}default_pricer_t;

default_pricer_t * ux_default_pricer_new(ux_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // PRICER_H
