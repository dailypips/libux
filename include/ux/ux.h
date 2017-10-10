/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

/** @file 
 * Libux C API header file.
 * The elements declared here constitute the Libux C API.
 */

#ifndef __UX_H__
#define __UX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ux/ux_common.h>
#include <ux/ux_memory.h>
#include <ux/ux_datetime.h>
#include <ux/ux_currency.h>
#include <ux/ux_events.h>

/* list all error code */
#define UX_OK   0

/* ids */
typedef uint16_t ux_iid_t;   /* for instrument id, range[1,65534] */
#define UX_INSTRUMENT_ID_MIN 1
#define UX_INSTRUMENT_ID_MAX 65534

typedef uint8_t ux_pid_t;   /* for provider id, range[1,254] */
//TODO: 可以取 -1
#define UX_UNKNOWN_PROVDER -1
#define UX_PROVIDER_ID_MIN 1
#define UX_PROVIDRE_ID_MAX 254

typedef ux_event_queue_t ux_event_queue_opened_t;
typedef ux_event_queue_t ux_event_queue_closed_t;

/* queue module */
typedef enum {
    UX_CATEGORY_MARKET = 0,
    UX_CATEGORY_EXECUTION,
    UX_CATEGORY_SERVICE,
    UX_CATEGORY_COMMAND,
    UX_CATEGORY_LAST
}ux_queue_category;

typedef struct ux_queue_s ux_queue_t;

UX_EXTERN ux_queue_t* ux_queue_new(unsigned int size, ux_queue_category category);
UX_EXTERN int ux_queue_push(ux_queue_t *q, void *e);

/* loop module */
typedef struct ux_loop_s ux_loop_t;
typedef struct ux_async_s ux_async_t;
typedef void (*ux_async_cb)(ux_loop_t *loop, void *data);

struct ux_async_s {
    ux_async_cb async_cb;
    void *data;
    /* private */
    ux_atomic_t next;
};

typedef enum {
    UX_BUS_SIMULATION,
    UX_BUS_REALTIME,
    UX_BUS_MODE_LAST
}ux_bus_mode;

typedef enum {
    UX_RUN_DEFAULT = 0,
    UX_RUN_ONCE,
    UX_RUN_NOWAIT
}ux_run_mode;

UX_EXTERN ux_loop_t* ux_loop_new(void);
UX_EXTERN void ux_loop_free(ux_loop_t *loop);
UX_EXTERN void ux_run(ux_loop_t*loop, ux_run_mode mode);
UX_EXTERN void ux_stop(ux_loop_t *loop);
UX_EXTERN void ux_wakeup(ux_loop_t* loop); 

/* instrument module */
typedef enum {
    UX_INSTRUMENT_TYPE_NONE = 0,
    UX_INSTRUMENT_TYPE_STOCK,
    UX_INSTRUMENT_TYPE_FUTURE,
    UX_INSTRUMENT_TYPE_OPTION,
    UX_INSTRUMENT_TYPE_FUTUREOPTION,
    UX_INSTRUMENT_TYPE_BOND,
    UX_INSTRUMENT_TYPE_FX,
    UX_INSTRUMENT_TYPE_INDEX,
    UX_INSTRUMENT_TYPE_ETF,
    UX_INSTRUMENT_TYPE_MULTILEG,
    UX_INSTRUMENT_TYPE_SYNTHETIC
}ux_instrument_type;

typedef enum {
    UX_PUT,
    UX_CALL
}ux_putcall_type;

typedef struct ux_instrument_s {
    ux_atomic_t refcount;
    int id;
    ux_instrument_type type;
    char* symbol;
    char* description;
    char* exchange;
    ux_currency_t currency;
    ux_currency_t ccy1;
    ux_currency_t ccy2;
    double tickSize;
    ux_time_t maturity;
    double factor;
    double strike;
    ux_putcall_type putcall;
    double margin;
    char* price_format;
    char* formula;
    //AltIdList altId = new AltIdList();
    void* legs[2];
    ux_event_trade_t trade;
    ux_event_ask_t ask;
    ux_event_bid_t bid;
    ux_event_bar_t bar;
    struct ux_instrument_s *parent;
    int is_deleted    :1;
    int is_persistent :1;

    void* data_provider;
    void* execution_provider;
}ux_instrument_t;

UX_EXTERN void ux_instrument_init(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_destory(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_ref(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_unref(ux_instrument_t *instrument);

/* order module */
typedef struct ux_order_s {
    ux_atomic_t refcount;
    int id;
    int clId;
    int algoId;
    int clientId;
    char* providerOrderId;
    char* clOrderId;
    uint8_t providerId;
    uint8_t routeId;
    int instrumentId;
    int portfolioId;
    int strategyId;
    ux_currency_t currencyId;
    ux_time_t transactTime;
    ux_order_status orderStatus;
    ux_order_side orderSide;
    ux_order_type orderType;
    ux_tif tif;
    ux_time_t expireTime;
    double price;
    double stopPx;
    double avgPx;
    double qty;
    double cumQty;
    double leavesQty;
    double minQty;
    double pegDifference;
    char* execInst;
    char* oca;
    char* text;
    char* account;
    char* clientID;
    double queueSize;
    int isOutPrice;
    int isQueueCalculated;
    void* commands[2]; // list
    void* reports[2]; // list
    //void* messages[2]; // list
} ux_order_t;

UX_EXTERN void ux_order_init(ux_order_t *order);
UX_EXTERN void ux_order_destory(ux_order_t *order);
UX_EXTERN void ux_order_ref(ux_order_t *order);
UX_EXTERN void ux_order_unref(ux_order_t *order);

/* provider */
typedef enum
{
    UX_PROVIDER_STATUS_CONNECTING,
    UX_PROVIDER_STATUS_CONNECTED,
    UX_PROVIDER_STATUS_DISCONNECTING,
    UX_PROVIDER_STATUS_DISCONNECTED
}ux_provider_status;

typedef struct ux_provider_s ux_provider_t;
typedef struct ux_data_provider_s ux_data_provider_t;
typedef struct ux_execution_provider_s ux_execution_provider_t;
typedef struct ux_fundamental_provider_s ux_fundamental_provider_t;
typedef struct ux_news_provider_s ux_news_provider_t;
typedef struct ux_historical_provider_s ux_historical_provider_t;
typedef struct ux_instrument_provider_s ux_instrument_provider_t;

typedef void (*ux_provider_cb)(ux_provider_t *provider, uintptr_t code);

#define UX_PROVIDER_PUBLIC_FIELDS \
    /* info */ \
    int (*is_enable)(ux_provider_t *provider); \
    int (*get_id)(ux_provider_t *provider); \
    const char* (*get_name)(ux_provider_t *provider); \
    /* method */    \
    ux_provider_status (*get_status)(ux_provider_t *provider); \
    void (*connect)(ux_provider_t *provider, ux_provider_cb on_connect); \
    void (*disconnect)(ux_provider_t *provider, ux_provider_cb on_disconnect);

struct ux_provider_s{
    UX_PROVIDER_PUBLIC_FIELDS
};

#define UX_DATA_PROVIDER_PUBLIC_FIELDS \
    UX_PROVIDER_PUBLIC_FIELDS   \
    void (*subscribe)  (ux_data_provider_t *provider, ux_instrument_t *instrument, int level2_flag, ux_provider_cb on_subscribe); \
    void (*unsubscribe)(ux_data_provider_t *provider, ux_instrument_t *instrument, ux_provider_cb on_unsubscribe);

struct ux_data_provider_s {
    UX_DATA_PROVIDER_PUBLIC_FIELDS
};

typedef struct ux_data_simulator_s ux_data_simulator_t;

#define UX_DATA_SIMULATOR_PUBLIC_FIELDS \
    UX_DATA_PROVIDER_PUBLIC_FIELDS \
    ux_time_t time1; \
    ux_time_t time2; \
    int run_on_subscribe:1; \
    int subscribe_bid:1; \
    int subscribe_ask:1; \
    int subscribe_trade:1; \
    int subscribe_bar:1; \
    int subscribe_level2:1; \
    int subscribe_news:1; \
    int subscribe_fundamental:1; \
    void* bar_filters[2]; \
    void* processors; \
    void* series[2]; \
    void (*run)(ux_data_simulator_t *simulator); \
    void (*clear)(ux_data_simulator_t *simulator);

struct ux_data_simulator_s {
    UX_DATA_SIMULATOR_PUBLIC_FIELDS
};


#define UX_FUNDAMENTAL_PROVIDER_PUBLIC_FIELDS \
    UX_PROVIDER_PUBLIC_FIELDS \
    void (*subscribe)  (ux_fundamental_provider_t *provider, ux_instrument_t *instrument, ux_provider_cb on_subscribe); \
    void (*unsubscribe)(ux_fundamental_provider_t *provider, ux_instrument_t *instrument, ux_provider_cb on_unsubscribe);

struct ux_fundamental_provider_s {
    UX_FUNDAMENTAL_PROVIDER_PUBLIC_FIELDS
};

#define UX_NEWS_PROVIDER_PUBLIC_FIELDS \
    UX_PROVIDER_PUBLIC_FIELDS \
    void (*subscribe)  (ux_news_provider_t *provider, ux_instrument_t *instrument, ux_provider_cb on_subscribe); \
    void (*unsubscribe)(ux_news_provider_t *provider, ux_instrument_t *instrument, ux_provider_cb on_unsubscribe);

struct ux_news_provider_s {
    UX_NEWS_PROVIDER_PUBLIC_FIELDS
};

#define UX_EXECUTION_PROVIDER_PUBLIC_FIELDS \
    UX_PROVIDER_PUBLIC_FIELDS \
    void (*send)(ux_execution_provider_t *provider, ux_event_execution_command_t *command, ux_provider_cb on_send);

struct ux_execution_provider_s {
    UX_EXECUTION_PROVIDER_PUBLIC_FIELDS
};

typedef enum {
    UX_COMMISSION_TYPE_PERSHARE,
    UX_COMMISSION_TYPE_PERCENT,
    UX_COMMISSION_TYPE_ABSOLUTE
}ux_commission_type;

#define UX_COMMISSION_PROVIDER_PUBLIC_FIELDS \
    ux_commission_type commission_type; \
    double commission; \
    double min_commission;

typedef struct ux_commission_provider_s {
    UX_COMMISSION_PROVIDER_PUBLIC_FIELDS
}ux_commission_provider_t;

#define UX_SLIPPAGE_PROVIDER_PUBLIC_FIELDS \
    double slippage;

typedef struct ux_slippage_provider_s {
    UX_SLIPPAGE_PROVIDER_PUBLIC_FIELDS
}ux_slippage_provider_t;

typedef struct ux_execution_simulator_s ux_execution_simulator_t;

#define UX_EXECUTION_SIMULATOR_PUBLIC_FIELDS \
    UX_EXECUTION_PROVIDER_PUBLIC_FIELDS \
    int fill_on_quote:1; \
    int fill_on_trade:1; \
    int fill_on_bar:1; \
    int fill_on_bar_open:1; \
    int fill_on_level2:1; \
    int fill_market_on_next:1; \
    int fill_limit_on_next:1; \
    int fill_stop_on_next:1; \
    int fill_stop_limit_on_next:1; \
    int fill_at_limit_price:1; \
    int fill_at_stop_price:1; \
    int partial_fills:1; \
    int queued:1; \
    void* bar_filters[2]; \
    ux_commission_provider_t *commission_provider; \
    ux_slippage_provider_t   *slippage_provider; \
    void (*on_bid)(ux_event_bid_t *bid); \
    void (*on_ask)(ux_event_bid_t *ask);  \
    void (*on_trade)(ux_event_trade_t *trade); \
    void (*on_level2_snapshot)(ux_event_l2snapshot_t *snapshot); \
    void (*on_level2_update)(ux_event_l2update_t *update); \
    void (*on_bar_open)(ux_event_bar_t *bar); \
    void (*on_bar)(ux_event_bar_t *bar); \
    void (*clear)(void);

struct ux_execution_simulator_s {
    UX_EXECUTION_SIMULATOR_PUBLIC_FIELDS
};

typedef struct {
    char* request_id;
    ux_instrument_t *instrument;
    ux_time_t start_time;
    ux_time_t end_time;
    ux_event_type data_type;
    ux_bar_type bar_type;
    long bar_size;      /* bar_size == -1 means no value */
}ux_historical_data_request_t;

#define UX_HISTORICAL_PROVIDER_PUBLIC_FIELDS \
    UX_PROVIDER_PUBLIC_FIELDS \
    void (*send)(ux_historical_provider_t *provider, ux_historical_data_request_t request, ux_provider_cb on_request); \
    void (*cancel)(ux_historical_provider_t *provider, char* request_id, ux_provider_cb on_cancel);

struct ux_historical_provider_s {
    UX_HISTORICAL_PROVIDER_PUBLIC_FIELDS
};

typedef struct ux_instrument_definition_request_s {
    char *request_id;
    ux_instrument_type filter_type;
    char *filter_symbol;
    char *filter_exchange;
}ux_instrument_definition_request_t;

#define UX_INSTRUMENT_PROVIDER_PUBLIC_FIELDS \
    UX_PROVIDER_PUBLIC_FIELDS \
    void (*send)(ux_instrument_provider_t *provider, ux_instrument_definition_request_t request, ux_provider_cb on_request); \
    void (*cancel)(ux_instrument_provider_t *provider, char* request_id, ux_provider_cb on_cancel);

struct ux_instrument_provider_s {
    UX_INSTRUMENT_PROVIDER_PUBLIC_FIELDS
};

typedef struct {
    ux_order_t *order;
    int order_id;
    int portfolio_id;
    int instrument_id;
    ux_currency_t currency;
    ux_order_side side;
    double qty;
    double price;
    char *text;
    double commission;
}ux_fill_t;

typedef enum {
    UX_ACCOUNT_DATA_TYPE_ACCTVALUE,
    UX_ACCOUNT_DATA_TYPE_POSITION,
    UX_ACCOUNT_DATA_TYPE_ORDER
}ux_accout_data_type;

#define PREDEFINE_ACCOUNT_FIELDS(_) \
    _(SYMBOL,    "Symbol") \
    _(EXCHANGE,   "Exchange") \
    _(SECURITY_TYPE, "SecurityType") \
    _(CURRENCY, "Currency") \
    _(MATURITY, "Maturity") \
    _(PUT_OR_CALL, "PutOrCall") \
    _(STRIKE, "Strike") \
    _(QTY, "Qty") \
    _(LONG_QTY, "LongQty") \
    _(SHORT_QTY, "ShortQty") \
    _(ORDER_ID, "OrderID") \
    _(ORDER_TYPE, "OrderType") \
    _(ORDER_SIDE, "OrderSide") \
    _(ORDER_STATUS, "OrderStatus") \
    _(PRICE, "Price") \
    _(STOP_PX, "StopPx")

typedef struct account_field_item_s {
    char *name;
    ux_currency_t currency;
    uintptr_t value;
}account_field_item_t;

typedef struct account_position_s {
    ux_currency_t currency;
    double value;
}account_position_t;

typedef struct ux_account_s ux_account_t;
struct ux_account_s {
    ux_currency_t currency;
    void* positions[2];
    void* transaction[2];
    struct ux_account_t *parent;
    int update_parent;
};

typedef struct ux_portfolio_s ux_portfolio_t;

typedef struct ux_fill_series_s {
    char *name;
    int count;
    ux_fill_t *min;
    ux_fill_t *max;
    ux_fill_t *fills;
}ux_fill_series_t;

typedef enum {
    UX_POSITION_SIDE_LOG,
    UX_POSITION_SIDE_SHORT
}ux_position_side;


typedef struct ux_position_s ux_position_t;

struct ux_position_s {
    ux_portfolio_t *portfolio;
    ux_instrument_t *instrument;
    int portfolio_id;
    int instrument_id;
    double amount;
    double qty_bought;
    double qty_sold;
    double open_value;
    double avg_price;
    double pnl;
    void* fills[2];
    ux_fill_t entry;
};

typedef struct ux_pricer_s {
    double (*get_price)(ux_position_t *position);
    double (*get_value)(ux_portfolio_t *position);
}ux_pricer_t;

typedef struct ux_time_series_item_s {
    ux_time_t timestamp;
    double value;
}ux_time_series_item_t;

typedef struct ux_time_series_s {
    int id;
    char *name;
    char *description;
    ux_time_series_item_t *min;
    ux_time_series_item_t *max;
    void* indicators[2];
    int count;
    ux_time_series_item_t *series;
}ux_time_series_t;

typedef struct ux_portfolio_performance_s {
    ux_portfolio_t *portfolio;
    int update_parent;
    ux_time_t time;
    double equity;
    double drawdown;
    double high_equity;
    ux_time_series_t enquity_series;
    ux_time_series_t drawdown_series;
}ux_portfolio_performance_t;

typedef struct ux_portfolio_statistics_s ux_portfolio_statistics_t;

typedef struct ux_portfolio_statistics_item_s {
    double total_value;
    double long_value;
    double short_value;
    ux_time_series_t totals;
    ux_time_series_t longs;
    ux_time_series_t shorts;
    ux_portfolio_t *portfolio;
    ux_portfolio_statistics_t *statistics;
    /* private */
    int emit_id;
}ux_portfolio_statistics_item_t;

typedef enum {
    UX_TRADE_DETECTION_TYPE_FIFO,
    UX_TRADE_DETECTION_TYPE_LIFO
}ux_trade_detection_type;

typedef struct ux_trade_detector_s {
    ux_instrument_t *instrument;
    ux_portfolio_t *portfolio;
    // TODO
}ux_trade_detector_t;


struct ux_portfolio_statistics_s {
    ux_portfolio_t *portfolio;
    void* trade_detectors[2];
    void* statistics[2];
};

typedef struct {
    ux_fill_t fill;
    int portfolio_id;
    ux_fill_series_t *fills;
    void* transactions[2];
    void* positions[2];
}ux_event_fill_t;

struct ux_portfolio_s {
    int id;
    char *name;
    char *description;
    ux_account_t *account;
    ux_fill_series_t fills;
    void* transactions[2];
    void* pisitions[2];
    ux_pricer_t pricer;
    ux_portfolio_performance_t *performance;
    ux_portfolio_statistics_t *statistics;
    int is_owner_algo;
    int is_loaded;
    ux_portfolio_t *parent;
    int update_parent;
    void* children[2];
};


typedef struct parameter_s {
    char *name;
    uintptr_t value;
    char *type_name;
}parameter_t;


typedef struct ux_strategy_s ux_strategy_t;

typedef enum {
    UX_STRATEGY_STATUS_RUNNING,
    UX_STRATEGY_STATUS_STOPPED
}ux_strategy_status;

typedef struct ux_tick_series_s {
    char *name;
    char *description;
    int count;
    ux_event_tick_t* ticks;
    ux_event_tick_t* min;
    ux_event_tick_t* max;
}ux_tick_series_t;

typedef struct ux_bar_series_s {
    int id;
    char *name;
    int count;
    ux_event_bar_t *bars;
    ux_event_bar_t *min;
    ux_event_bar_t *max;
    void* indicators[2];
}ux_bar_series_t;

typedef enum {
    UX_STOP_FILL_MODE_CLOSE,
    UX_STOP_FILL_MODE_HIGHLOW,
    UX_STOP_FILL_MODE_STOP
}ux_stop_fill_mode;

typedef enum {
    UX_STOP_MODE_ABSOLUTE,
    UX_STOP_MODE_PERCENT
}ux_stop_mode;

typedef enum {
    UX_STOP_STATUS_ACTIVE,
    UX_STOP_STATUS_EXECUTED,
    UX_STOP_STATUS_CANCELED
}ux_stop_status;

typedef enum {
    UX_STOP_TYPE_FIXED,
    UX_STOP_TYPE_TRAILING,
    UX_STOP_TYPE_TIME
}ux_stop_type;

typedef struct ux_stop_s {
    ux_strategy_t *strategy;
    ux_position_t *position;
    ux_instrument_t *instrument;
    int connected;
    ux_stop_type type;
    ux_stop_mode mode;
    ux_stop_status status;
    double level;
    double init_price;
    double current_price;
    double stop_price;
    double fill_price;
    double trail_price;
    double qty;
    ux_position_side side;
    ux_time_t creation_time;
    ux_time_t completion_time;
    int trace_on_quote :1;
    int trace_on_trade:1;
    int trace_on_bar:1;
    int trace_on_bar_open:1;
    int trail_on_open:1;
    int trail_on_highlow:1;
    long filter_bar_size;
    ux_bar_type filter_bar_type;
    ux_stop_fill_mode fill_mode;
}ux_stop_t;

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

typedef enum {
    UX_STRATEGY_MODE_BACKTEST,
    UX_STRATEGY_MODE_PAPER,
    UX_STRATEGY_MODE_LIVE
}ux_strategy_mode;

struct ux_strategy_s {
    int raise_events;
    int id;
    char *name;
    int client_id;
    int enabled;
    ux_strategy_status status;
    ux_portfolio_t *portfolio;
    void* instruments[2];
    ux_data_provider_t *data_provider;
    ux_execution_provider_t *execution_provider;
    ux_fundamental_provider_t *fundamental_provider;
    void* children[2];
    ux_strategy_t *parent;
    ux_tick_series_t bids;
    ux_tick_series_t asks;
    ux_tick_series_t trade;
    ux_bar_series_t bars;
    ux_time_series_t equity;
    void* stops[2];
    void* subscritions[2];
};

#ifdef __cplusplus
}
#endif

#endif /*__UX_H__*/
