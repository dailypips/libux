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
    /*
  IDataProvider dataProvider;
  IExecutionProvider executionProvider;
  */
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
    void* messages[2]; // list

    /*
    Instrument instrument;
    IExecutionProvider executionProvider;
    Portfolio portfolio;
    */
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
typedef struct ux_execution_provder_s ux_execution_provder_t;
typedef struct ux_fundamental_provider_s ux_fundamental_provider_t;
typedef struct ux_news_provider_s ux_news_provider_t;
typedef struct ux_historical_provider_s ux_historical_provider_t;
typedef struct ux_instrument_provider_s ux_instrument_provider_t;

typedef void (*provider_cb)(ux_provider_t *provider, int code);

#define UX_PROVIDER_PUBLIC_FIELDS \
    /* info */ \
    int (*is_enable)(ux_provider_t *provider); \
    int (*get_id)(ux_provider_t *provider); \
    const char* (*get_name)(ux_provider_t *provider); \
    /* method */    \
    ux_provider_status (*get_status)(ux_provider_t *provider); \
    void (*connect)(ux_provider_t *provider, provider_cb on_connect); \
    void (*disconnect)(ux_provider_t *provider, provider_cb on_disconnect);

struct ux_provider_s{
    UX_PROVIDER_PUBLIC_FIELDS
};

struct ux_data_provider_s {
    UX_PROVIDER_PUBLIC_FIELDS
    void (*subscribe)(ux_data_provider_t *provider, ux_instrument_t *instrument, provider_cb on_subscribe);
    void (*unsubscribe)(ux_data_provider_t *provider, ux_instrument_t *instrument, provider_cb on_unsubscribe);
};

struct ux_execution_provder_s {
    UX_PROVIDER_PUBLIC_FIELDS
    void (*send)(ux_execution_provder_t *provider, ux_event_execution_command_t *command, provider_cb on_send);
};

struct ux_fundamental_provider_s {
    UX_PROVIDER_PUBLIC_FIELDS
    void (*subscribe)(ux_fundamental_provider_t *provider, ux_instrument_t *instrument, provider_cb on_subscribe);
    void (*unsubscribe)(ux_fundamental_provider_t *provider, ux_instrument_t *instrument, provider_cb on_unsubscribe);
};

struct ux_news_provider_s {
    UX_PROVIDER_PUBLIC_FIELDS
    void (*subscribe)(ux_news_provider_t *provider, ux_instrument_t *instrument, provider_cb on_subscribe);
    void (*unsubscribe)(ux_news_provider_t *provider, ux_instrument_t *instrument, provider_cb on_unsubscribe);
};

typedef struct {
    char* request_id;
    ux_instrument_t *instrument;
    ux_time_t time1;
    ux_time_t time2;
    uint8_t data_type;
    ux_bar_type bar_type;
    long bar_size;      /* bar_size == -1 means no value */
}ux_historical_data_request_t;

struct ux_historical_provider_s {
    UX_PROVIDER_PUBLIC_FIELDS
    void (*send)(ux_historical_provider_t *provider, ux_historical_data_request_t request, provider_cb on_request);
    void (*cancel)(ux_historical_provider_t *provider, char* request_id, provider_cb on_cancel);
};

typedef struct ux_instrument_definition_request_s {
    char *id;
    ux_instrument_type filter_type;
    char *filter_symbol;
    char *filter_exchange;
}ux_instrument_definition_request_t;

struct ux_instrument_provider_s {
    UX_PROVIDER_PUBLIC_FIELDS
    void (*send)(ux_instrument_provider_t *provider, ux_historical_data_request_t request, provider_cb on_request);
    void (*cancel)(ux_instrument_provider_t *provider, char* request_id, provider_cb on_cancel);
};

#ifdef __cplusplus
}
#endif

#endif /*__UX_H__*/
