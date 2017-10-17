/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_PORTFOLIO_H__
#define __UX_PORTFOLIO_H__

#include <ux/common.h>
#include <ux/datetime.h>
#include <ux/currency.h>
#include <ux/instrument.h>
#include <ux/order.h>
#include <ux/account.h>
#include <ux/position.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UX_ACCOUNT_DATA_TYPE_ACCTVALUE,
    UX_ACCOUNT_DATA_TYPE_POSITION,
    UX_ACCOUNT_DATA_TYPE_ORDER
}ux_account_data_type;

#define PREDEFINE_ACCOUNT_FIELDS(_)    \
    /* CONSTANTS       | NAME */       \
    _(SYMBOL,         "Symbol")        \
    _(EXCHANGE,       "Exchange")      \
    _(SECURITY_TYPE,  "SecurityType")  \
    _(CURRENCY,       "Currency")      \
    _(MATURITY,       "Maturity")      \
    _(PUT_OR_CALL,    "PutOrCall")     \
    _(STRIKE,         "Strike")        \
    _(QTY,            "Qty")           \
    _(LONG_QTY,       "LongQty")       \
    _(SHORT_QTY,      "ShortQty")      \
    _(ORDER_ID,       "OrderID")       \
    _(ORDER_TYPE,     "OrderType")     \
    _(ORDER_SIDE,     "OrderSide")     \
    _(ORDER_STATUS,   "OrderStatus")   \
    _(PRICE,          "Price")         \
    _(STOP_PX,        "StopPx")

typedef struct account_field_item_s {
    char *name;
    ux_currency_t currency;
    uintptr_t value;
}account_field_item_t;

typedef struct account_position_s {
    ux_currency_t currency;
    double value;
}account_position_t;


typedef struct ux_fill_series_s {
    char *name;
    int count;
    ux_fill_t *min;
    ux_fill_t *max;
    ux_fill_t *fills;
}ux_fill_series_t;

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
}uxe_fill_t;

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


#ifdef __cplusplus
}
#endif

#endif // __UX_PORTFOLIO_H__
