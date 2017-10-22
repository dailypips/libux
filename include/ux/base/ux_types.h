/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_TYPES_H__
#define __UX_TYPES_H__

#include <ux/base/ux_common.h>
#include <ux/base/ux_datetime.h>
#include <ux/base/ux_currency.h>
#include <ux/base/ux_atomic.h>
#include <ux/base/ux_memory.h>
#include <ux/base/ux_idarray.h>

#include <ux/event/event_def.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ids */
typedef uint16_t ux_iid_t; /* for instrument id, range[1..65534] */
#define UX_INSTRUMENT_ID_MIN 1
#define UX_INSTRUMENT_ID_MAX 65534

typedef uint8_t ux_pid_t; /* for provider id, range[1..254] */
typedef uint8_t ux_rid_t; /* for route id, range[1..254] */
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

/* list all error code */
#define UX_OK 0

/* enums */

typedef enum {
    UX_L2_ACTION_NEW,
    UX_L2_ACTION_CHANGE,
    UX_L2_ACTION_DELETE,
    UX_L2_ACTION_RESET
} ux_level2_action;

typedef enum {
    UX_L2SIDE_BID,
    UX_L2SIDE_ASK,
} ux_level2_side;


typedef enum {
    UX_EXEC_NEW,
    UX_EXEC_STOPPED,
    UX_EXEC_REJECTED,
    UX_EXEC_EXPIRED,
    UX_EXEC_TRADE,
    UX_EXEC_PENDING_CANCEL,
    UX_EXEC_CANCELLED,
    UX_EXEC_CANCEL_REJECT,
    UX_EXEC_PENDING_REPLACE,
    UX_EXEC_REPLACE,
    UX_EXEC_REPLACE_REJECT,
    UX_EXEC_TRADE_CORRECT,
    UX_EXEC_TRADE_CANCEL,
    UX_EXEC_ORDER_STATUS,
    UX_EXEC_PENDING_NEW,
    UX_EXEC_CLEARING_HOLD
} ux_exec_type;

typedef enum {
    UX_ORDER_TYPE_MARKET,
    UX_ORDER_TYPE_STOP,
    UX_ORDER_TYPE_LIMIT,
    UX_ORDER_TYPE_STOP_LIMIT,
    UX_ORDER_TYPE_MARKET_CLOSE,
    UX_ORDER_TYPE_PEGGED,
    UX_ORDER_TYPE_TRAILING_STOP,
    UX_ORDER_TYPE_TRAILING_STOP_LIMIT
} ux_order_type;

typedef enum {
    UX_ORDER_STATUS_NOT_SENT,
    UX_ORDER_STATUS_PENDING_NEW,
    UX_ORDER_STATUS_NEW,
    UX_ORDER_STATUS_REJECTED,
    UX_ORDER_STATUS_PARTIALLY_FILLED,
    UX_ORDER_STATUS_FILLED,
    UX_ORDER_STATUS_PENDING_CANCEL,
    UX_ORDER_STATUS_CANCELLED,
    UX_ORDER_STATUS_EXPIRED,
    UX_ORDER_STATUS_PENDING_REPLACE,
    UX_ORDER_STATUS_REPLACED
} ux_order_status;

typedef enum {
    UX_TIF_ATC,
    UX_TIF_DAY,
    UX_TIF_GTC,
    UX_TIF_IOC,
    UX_TIF_OPG,
    UX_TIF_OC,
    UX_TIF_FOK,
    UX_TIF_GTX,
    UX_TIF_GTD,
    UX_TIF_GFS,
    UX_TIF_AUC
} ux_tif;

typedef enum {
    UX_ORDER_SIDE_BUY,
    UX_ORDER_SIDE_SELL
} ux_order_side;

typedef enum {
    UX_STRATEGY_STATUS_TYPE_STARTED,
    UX_STRATEGY_STATUS_TYPE_STOPPED
}ux_strategy_status_type;

typedef struct ux_strategy_status_info_s {
    ux_strategy_status_type type;
    char *solution;
    char *mode;
}ux_strategy_status_info_t;

typedef enum {
    UX_STRATEGY_PERSISTENCE_NONE,
    UX_STRATEGY_PERSISTENCE_FULL,
    UX_STRATEGY_PERSISTENCE_SAVE,
    UX_STRATEGY_PERSISTENCE_LOAD
}ux_strategy_persistence;


#ifdef __cplusplus
}
#endif

#endif // __UX_TYPES_H__
