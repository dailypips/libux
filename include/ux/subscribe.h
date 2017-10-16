/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef SUBSCRIBE_H
#define SUBSCRIBE_H

#include <ux/common.h>
#include <ux/types.h>
#include <ux/currency.h>
#include <ux/event.h>
#include <ux/position.h>
#include <ux/provider.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ux_subscription_s {
    int request_id;
    char *symbol;
    int source_id;
    ux_instrument_t *instrument;
    ux_data_provider_t *provider;
    int instrument_id;
    int provider_id;
    int route_id;
}ux_subscription_t;

#ifdef __cplusplus
}
#endif

#endif // SUBSCRIBE_H
