/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef STRATEGY_H
#define STRATEGY_H

#include <ux/common.h>
#include <ux/types.h>
#include <ux/currency.h>
#include <ux/event.h>
#include <ux/position.h>
#include <ux/provider.h>
#include <ux/series.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct parameter_s {
    char *name;
    uintptr_t value;
    char *type_name;
}parameter_t;


typedef enum {
    UX_STRATEGY_MODE_BACKTEST,
    UX_STRATEGY_MODE_PAPER,
    UX_STRATEGY_MODE_LIVE
}ux_strategy_mode;

typedef enum {
    UX_STRATEGY_STATUS_RUNNING,
    UX_STRATEGY_STATUS_STOPPED
}ux_strategy_status;

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

    /* cb */
    void (*add_strategy)(ux_strategy_t *parent, ux_strategy_t *strategy);
    void (*remove_strategy)(ux_strategy_t *parent, ux_strategy_t *strategy);
    void (*on_provider_connected)(ux_strategy_t *strategy, ux_provider_t *provider);
};

#ifdef __cplusplus
}
#endif

#endif // STRATEGY_H
