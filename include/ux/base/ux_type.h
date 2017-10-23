/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_TYPES_H__
#define __UX_TYPES_H__

#include <ux/base/ux_common.h>
#include <ux/base/ux_currency.h>
#include <ux/base/ux_datetime.h>
#include <ux/base/ux_idarray.h>
#include <ux/base/ux_memory.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ids */
typedef int16_t  ux_iid_t;   /* instrument */
typedef int32_t  ux_oid_t;   /* order id */
typedef int16_t  ux_pid_t;   /* provider id */
typedef int32_t  ux_id_t;   /* strategy/portfolio/position id*/

/* atomic type */
typedef intptr_t ux_atomic_t;

typedef struct ux_ctx_s ux_ctx_t;

typedef struct ux_event_s ux_event_t;
typedef struct ux_event_tick_s ux_event_tick_t;

typedef struct uxe_logger_s ux_event_logger_t;

typedef struct ux_instrument_s ux_instrument_t;
typedef struct ux_order_s ux_order_t;

typedef struct ux_fill_s ux_fill_t;
typedef struct ux_transaction_s ux_transaction_t;

typedef struct ux_position_s ux_position_t;
typedef struct ux_account_s ux_account_t;
typedef struct ux_portfolio_s ux_portfolio_t;

typedef struct ux_provider_s ux_provider_t;

typedef struct ux_stop_s ux_stop_t;
typedef struct ux_strategy_s ux_strategy_t;

typedef struct ux_indicator_s ux_indicator_t;

typedef struct ux_fill_series_s ux_fill_series_t;
typedef struct ux_time_series_s ux_time_series_t;
typedef struct ux_tick_series_s ux_tick_series_t;
typedef struct ux_bar_series_s ux_bar_series_t;

/* list all error code */
#define UX_OK   0
#define UX_ERR -1

#ifdef __cplusplus
}
#endif

#endif // __UX_TYPES_H__
