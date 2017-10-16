/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_TYPES_H__
#define __UX_TYPES_H__

#include <ux/common.h>
#include <ux/event_def.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t ux_time_t;
typedef int64_t  ux_timespan_t;

typedef intptr_t ux_atomic_t;

/* ids */
typedef uint16_t ux_iid_t;   /* for instrument id, range[1,65534] */
#define UX_INSTRUMENT_ID_MIN 1
#define UX_INSTRUMENT_ID_MAX 65534

typedef uint8_t ux_pid_t;   /* for provider id, range[1,254] */
//TODO: 可以取 -1
#define UX_UNKNOWN_PROVDER -1
#define UX_PROVIDER_ID_MIN 1
#define UX_PROVIDRE_ID_MAX 254

typedef struct ux_ctx_s ux_ctx_t;
/* all events */
typedef struct ux_event_s ux_event_t;
typedef struct ux_event_tick_s ux_event_tick_t;

typedef struct ux_instrument_s ux_instrument_t;
typedef struct ux_order_s ux_order_t;

typedef struct ux_fill_s ux_fill_t;
typedef struct ux_transaction_s ux_transaction_t;

typedef struct ux_position_s ux_position_t;
typedef struct ux_account_s ux_account_t;
typedef struct ux_portfolio_s ux_portfolio_t;

typedef struct ux_provider_s ux_provider_t;

typedef struct ux_strategy_s ux_strategy_t;

typedef struct ux_ctx_s ux_ctx_t;
typedef struct ux_async_s ux_async_t;
typedef void (*ux_async_cb)(ux_ctx_t *ctx, void *data);




#define EVENTETYPE(name, lname, destory, clone, processor)	typedef struct uxe_##lname##_s uxe_##lname##_t;
EVENTDEF(EVENTETYPE)
#undef EVENTETYPE

/* list all error code */
#define UX_OK   0


#ifdef __cplusplus
}
#endif

#endif // __UX_TYPES_H__
