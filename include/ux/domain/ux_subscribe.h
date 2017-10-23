/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_SUBSCRIBE_H__
#define __UX_SUBSCRIBE_H__

#include <ux/base/ux_common.h>
#include <ux/base/ux_currency.h>
#include <ux/event/ux_event.h>
#include <ux/domain/ux_position.h>
#include <ux/domain/ux_provider.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ux_subscription_s {
    int request_id;
    char *symbol;
    int source_id;
    ux_instrument_t *instrument;
    ux_data_provider_t *provider;
    ux_iid_t instrument_id;
    ux_pid_t provider_id;
    ux_pid_t route_id;
}ux_subscription_t;

#ifdef __cplusplus
}
#endif

#endif // __UX_SUBSCRIBE_H__
