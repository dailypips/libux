/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_PROVIDER_H__
#define __UX_PROVIDER_H__

#include <ux/common.h>
#include <ux/datetime.h>
#include <ux/currency.h>
#include <ux/instrument.h>
#include <ux/order.h>

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct ux_bar_filter_item_s {
    void* queue_node[2];
    ux_bar_type bar_type;
    long bar_size;
}ux_bar_filter_item_t;

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
    void (*send)(ux_execution_provider_t *provider, uxe_execution_command_t *command, ux_provider_cb on_send);

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

typedef enum {
    UX_EXECUTION_SIMULATOR_FLAG_FILL_ON_QUOTE           = 1 << 0,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_ON_TRADE           = 1 << 1,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_ON_BAR             = 1 << 2,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_ON_BAR_OPEN        = 1 << 3,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_ON_LEVEL2          = 1 << 4,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_MARKET_ON_NEXT     = 1 << 5,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_LIMIT_ON_NEXT      = 1 << 6,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_STOP_ON_NEXT       = 1 << 7,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_STOP_LIMIT_ON_NEXT = 1 << 8,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_AT_LIMIT_PRICE     = 1 << 9,
    UX_EXECUTION_SIMULATOR_FLAG_FILL_AT_STOP_PRICE      = 1 << 10,
    UX_EXECUTION_SIMULATOR_FLAG_PARTIAL                 = 1 << 11,
    UX_EXECUTION_SIMULATOR_FLAG_QUEUED                  = 1 << 12
}ux_execution_simulator_flag_t;

#define UX_EXECUTION_SIMULATOR_PUBLIC_FIELDS \
    UX_EXECUTION_PROVIDER_PUBLIC_FIELDS \
    int fill_flag; \
    void* bar_filters[2]; \
    ux_commission_provider_t *commission_provider; \
    ux_slippage_provider_t   *slippage_provider; \
    void (*on_bid)(ux_execution_simulator_t*simulator, uxe_bid_t *bid); \
    void (*on_ask)(ux_execution_simulator_t*simulator, uxe_bid_t *ask);  \
    void (*on_trade)(ux_execution_simulator_t*simulator, uxe_trade_t *trade); \
    void (*on_level2_snapshot)(ux_execution_simulator_t*simulator, uxe_level2_snapshot_t *snapshot); \
    void (*on_level2_update)(ux_execution_simulator_t*simulator, uxe_level2_update_t *update); \
    void (*on_bar_open)(ux_execution_simulator_t*simulator, uxe_bar_t *bar); \
    void (*on_bar)(ux_execution_simulator_t*simulator, uxe_bar_t *bar); \
    void (*clear)(ux_execution_simulator_t*simulator);

struct ux_execution_simulator_s {
    UX_EXECUTION_SIMULATOR_PUBLIC_FIELDS
};

typedef struct {
    char* request_id;
    ux_instrument_t *instrument;
    ux_time_t start_time;
    ux_time_t end_time;
    uxe_type data_type;
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

#ifdef __cplusplus
}
#endif

#endif // __UX_PROVIDER_H__
