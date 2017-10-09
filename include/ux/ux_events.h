/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#ifndef __UX_EVENTS_H__
#define __UX_EVENTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ux/ux_common.h>
#include <ux/ux_datetime.h>

/* event module */
#define EVENTDEF(_)                                                                                   \
    /* UppercaseName        LowcaseName         destory            clone              dispatch*/      \
     /* market events */                                                                              \
    _(REMINDER,             reminder,           default,           default,           reminder)       \
    _(ASK,                  ask,                default,           default,           ask)            \
    _(BID,                  bid,                default,           default,           bid)            \
    _(TRADE,                trade,              default,           default,           trade)          \
    _(L2SNAPSHOT,           l2snapshot,         l2snapshot,        l2snapshot,        default)        \
    _(L2UPDATE,             l2update,           l2update,          l2update,          default)        \
    _(BAR,                  bar,                default,           default,           default)        \
    _(NEWS,                 news,               news,              news,              default)        \
    _(FUNDAMENTAL,          fundamental,        default,           default,           default)        \
    /* execution events */                                                                            \
    _(ACCOUNT_REPORT,       account_report,     account_report,    account_report,    default)        \
    _(EXECUTION_REPORT,     execution_report,   execution_report,  execution_report,  default)        \
    _(EXECUTION_COMMAND,    execution_command,  execution_command, execution_command, default)        \
    /* internal events */                                                                             \
    _(QUEUE_OPENED,         queue_opened,       default,           default,           default)        \
    _(QUEUE_CLOSED,         queue_closed,       default,           default,           default)        \
    _(SIMULATOR_START,      simulator_start,    default,           default,           default)        \
    _(SIMULATOR_STOP,       simulator_stop,     default,           default,           default)        \
    _(SIMULATOR_PROGRESS,   simulator_progress, default,           default,           default)        \
    _(EXCEPTION,            exception,          exception,         exception,         default)


typedef enum {
#define EVENTENUM(name, lname, destory, clone, processor)	UX_EVENT_##name,
EVENTDEF(EVENTENUM)
#undef EVENTENUM
    UX_EVENT_LAST
} ux_event_type;

#ifdef NDEBUG
#define UX_EVENT_PUBLIC_FIELDS \
    ux_atomic_t refcount;        \
    ux_event_type type;        \
    ux_time_t timestamp;
#else
#define UX_EVENT_PUBLIC_FIELDS \
    ux_atomic_t refcount;        \
    ux_event_type type;        \
    ux_time_t timestamp; \
    void* dummy;
#endif

typedef struct ux_event_s {
  UX_EVENT_PUBLIC_FIELDS
}ux_event_t;

UX_EXTERN size_t ux_event_size(ux_event_type type);
UX_EXTERN ux_event_t* ux_event_malloc(ux_event_type type);
UX_EXTERN ux_event_t* ux_event_ref(ux_event_t *e);
UX_EXTERN void ux_event_unref(ux_event_t *e);
UX_EXTERN ux_event_t* ux_event_clone(ux_event_t *e);

/* events */
/* reminder event */
typedef enum {
    UX_CLOCK_LOCAL  = 0,
    UX_CLOCK_EXCHANGE,
    UX_CLOCK_LAST
}ux_clock_type;

typedef struct ux_event_reminder_s ux_event_reminder_t;

typedef void (*ux_reminder_cb)(ux_event_reminder_t *timer);

struct ux_event_reminder_s {
    UX_EVENT_PUBLIC_FIELDS
    ux_clock_type clock_type;
    ux_reminder_cb callback;
    void *user_data;
    ux_time_t timeout;
    uint64_t repeat;
    /*private*/
    void *loop;
    void* heap_node[3];
    uint64_t start_id;
};

/* market data event */
#define UX_EVENT_COMMON_FIELDS \
    int provider;           \
    int instrument;

#define UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME \
    UX_EVENT_COMMON_FIELDS \
    ux_time_t exchange_timestamp;

typedef struct tick_event_s {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    double price;
    long size;
} ux_event_tick_t;

typedef ux_event_tick_t ux_event_ask_t;
typedef ux_event_tick_t ux_event_bid_t;
typedef ux_event_tick_t ux_event_trade_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS
    char *urgency;
    char *url;
    char *headline;
    char *text;
} ux_event_news_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS
} ux_event_fundamental_t;

typedef struct {
    double price;
    long size;
}ux_tick_info_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    size_t num_bids;
    size_t num_asks;
    ux_tick_info_t* ticks;
}ux_event_l2snapshot_t;

typedef enum {
    UX_L2_ACTION_NEW,
    UX_L2_ACTION_CHANGE,
    UX_L2_ACTION_DELETE,
    UX_L2_ACTION_RESET
}ux_level2_action;

typedef enum {
    UX_L2SIDE_BID,
    UX_L2SIDE_ASK,
}ux_level2_side;

typedef struct {
    ux_level2_side side;
    ux_level2_action action;
    int position;
}ux_level2_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    size_t num_entries;
    ux_level2_t *entries;
}ux_event_l2update_t;

typedef enum {
    UX_BAR_TYPE_TIME,
    UX_BAR_TYPE_TICK,
    UX_BAR_TYPE_VOLUME,
    UX_BAR_TYPE_RANGE,
    UX_BAR_TYPE_SESSION,
    UX_BAR_TYPE_RENKO,
    UX_BAR_TYPE_CUSTOM
}ux_bar_type;

typedef enum {
    UX_BAR_STATUS_INCOMPLETE,
    UX_BAR_STATUS_COMPLETE,
    UX_BAR_STATUS_OPEN,
    UX_BAR_STATUS_HIGH,
    UX_BAR_STATUS_LOW,
    UX_BAR_STATUS_CLOSE
}ux_bar_status;

typedef enum {
    UX_BAR_INPUT_TRADE,
    UX_BAR_INPUT_BID,
    UX_BAR_INPUT_ASK,
    UX_BAR_INPUT_MIDDLE,
    UX_BAR_INPUT_TICK,
    UX_BAR_INPUT_BIDASK
}ux_bar_input;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    ux_bar_type bar_type;
    long size;
    ux_time_t open_time;
    double high;
    double low;
    double open;
    double close;
    long volume;
    long open_int;
    long tick_count;
    ux_bar_status status;
}ux_event_bar_t;

static double ux_bar_get_range(ux_event_bar_t *bar)
{
    return bar->high - bar->low;
}

static double ux_bar_get_median(ux_event_bar_t *bar)
{
    return (bar->high + bar->low) / 2.0;
}

static double ux_bar_get_typical(ux_event_bar_t *bar)
{
    return (bar->high + bar->low + bar->close) / 3.0;
}

static double ux_bar_get_weighted(ux_event_bar_t *bar)
{
    return (bar->high + bar->low + bar->close * 2.0) / 4.0;
}

static double ux_bar_get_average(ux_event_bar_t *bar)
{
    return (bar->high + bar->low + bar->open+ bar->close) / 4.0;
}

/* execution */
#define UX_EVENT_EXECUTION_PUBLIC_FIELDS \
    int provider;   \
    int instrument; \
    int id; \
    int order_id; \
    char *client_order_id; \
    char *provider_order_id; \
    int client_id; \
    int is_loadded; \
    ux_currency_t currency; \

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_EXECUTION_PUBLIC_FIELDS
    int portfolio;
    double amount;
    char *text;
}ux_event_account_report_t;


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
}ux_exec_type;

typedef enum {
    UX_ORDER_TYPE_MARKET,
    UX_ORDER_TYPE_STOP,
    UX_ORDER_TYPE_LIMIT,
    UX_ORDER_TYPE_STOP_LIMIT,
    UX_ORDER_TYPE_MARKET_CLOSE,
    UX_ORDER_TYPE_PEGGED,
    UX_ORDER_TYPE_TRAILING_STOP,
    UX_ORDER_TYPE_TRAILING_STOP_LIMIT
}ux_order_type;

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
}ux_order_status;

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
}ux_tif;

typedef enum {
    UX_SIDE_BUY,
    UX_SIDE_SELL
}ux_order_side;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_EXECUTION_PUBLIC_FIELDS
    int route_id;
    int algo;
    int portfolio;
    int strategy_id;
    ux_time_t transact_time;
    double min_qty;
    double peg_difference;
    char* exec_inst;
    char* oca;
    char* account;
    char* clientID;
    ux_exec_type exec_type;
    ux_order_type order_type;
    ux_order_side order_side;
    ux_tif tif;
    ux_time_t expire_time;
    ux_order_status order_status;
    double last_price;
    double avg_price;
    double order_qty;
    double cum_qty;
    double last_qty;
    double leaves_qty;
    double price;
    double stop_price;
    double commission;
    char* text;
    char* exec_id;
}ux_event_execution_report_t;


typedef enum {
    UX_EXECUTION_COMMAND_SEND,
    UX_EXECUTION_COMMAND_CANCEL,
    UX_EXECUTION_COMMAND_REPLACE
}ux_execution_command_type;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_EXECUTION_PUBLIC_FIELDS
    int route_id;
    ux_execution_command_type execution_command_type;
    char* symbol;
    int algo;
    int portfolio;
    int strategy_id;
    ux_time_t transact_time;
    ux_order_side order_side;
    ux_order_type order_type;
    ux_tif tif;
    double price;
    double stop_price;
    double qty;
    double min_qty;
    double peg_difference;
    char* exec_inst;
    char* oca;
    char* text;
    char* account;
    char* clientID;
}ux_event_execution_command_t;

/* system */
typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    void *q;
}ux_event_queue_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_start_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_step_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_stop_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_resume_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_paused_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    ux_time_t time1;
    ux_time_t time2;
    long long count;
}ux_event_simulator_start_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_simulator_stop_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_simulator_progress_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    char *source;
}ux_event_exception_t;

#ifdef __cplusplus
}
#endif

#endif /*__UX_EVENTS_H__*/
