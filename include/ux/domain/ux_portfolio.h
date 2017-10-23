/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_PORTFOLIO_H__
#define __UX_PORTFOLIO_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_series.h>

#ifdef __cplusplus
extern "C" {
#endif

//TODO series 直接嵌入到字段里，而不是现在的指针
typedef struct ux_pricer_s {
    double (*get_price)(ux_position_t* position);
    double (*get_value)(ux_portfolio_t* position);
} ux_pricer_t;

typedef struct ux_portfolio_performance_s {
    ux_portfolio_t* portfolio;
    int update_parent;
    ux_time_t time;
    double equity;
    double drawdown;
    double high_equity;
    ux_time_series_t enquity_series;
    ux_time_series_t drawdown_series;
} ux_portfolio_performance_t;

typedef struct ux_portfolio_statistics_s ux_portfolio_statistics_t;

typedef struct ux_portfolio_statistics_item_s {
    double total_value;
    double long_value;
    double short_value;
    ux_time_series_t *totals;
    ux_time_series_t *longs;
    ux_time_series_t *shorts;
    ux_portfolio_t* portfolio;
    ux_portfolio_statistics_t* statistics;
    /* private */
    int emit_id;
} ux_portfolio_statistics_item_t;

typedef enum {
    UX_TRADE_DETECTION_TYPE_FIFO,
    UX_TRADE_DETECTION_TYPE_LIFO
} ux_trade_detection_type;

typedef struct ux_trade_detector_s {
    ux_instrument_t* instrument;
    ux_portfolio_t* portfolio;
    // TODO
} ux_trade_detector_t;

struct ux_portfolio_statistics_s {
    ux_portfolio_t* portfolio;
    void* trade_detectors[2];
    void* statistics[2];
};

/*typedef struct {
    ux_fill_t fill;
    int portfolio_id;
    ux_fill_series_t* fills;
    void* transactions[2];
    void* positions[2];
} uxe_fill_t;*/

struct ux_portfolio_s {
    ux_ctx_t *ctx;
    ux_instrument_t *instrument;
    ux_id_t id;
    char* name;
    char* description;
    ux_account_t* account;
    ux_fill_series_t *fills;
    void* transactions[2];
    void* positions[2];
    ux_idarray_t transaction_by_order_id;
    ux_idarray_t position_by_instrument_id;
    ux_pricer_t pricer;
    ux_portfolio_performance_t* performance;
    ux_portfolio_statistics_t* statistics;
    int is_owner_algo;
    int is_loaded;
    ux_portfolio_t* parent;
    int update_parent;
    void* children[2];
};

#ifdef __cplusplus
}
#endif

#endif // __UX_PORTFOLIO_H__
