/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_STRATEGY_H__
#define __UX_STRATEGY_H__

#include <ux/base/ux_common.h>
#include <ux/base/ux_currency.h>
#include <ux/event/ux_event.h>
#include <ux/domain/ux_position.h>
#include <ux/domain/ux_provider.h>
#include <ux/domain/ux_series.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ux_strategy_s ux_strategy_t;

typedef struct parameter_s {
    char *name;
    uintptr_t value;
    char *type_name;
}parameter_t;

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

    /* virtual */
    void (*on_strategy_init)(ux_strategy_t* strategy);
    void (*on_strategy_start)(ux_strategy_t* strategy);
    void (*on_strategy_stop)(ux_strategy_t* strategy);
    void (*on_strategy_event)(ux_strategy_t* strategy, void* data);
    /* exception process */
    void (*on_exception)(ux_strategy_t* strategy, const char* source, ux_event_t* e, uxe_exception_t* exception);

    /* instrument */
    void (*on_instrument_added)(ux_strategy_t* strategy, ux_instrument_t* instrument);
    void (*on_instrument_removed)(ux_strategy_t* strategy, ux_instrument_t* instrument);
    /* reminder */
    void (*on_reminder)(ux_strategy_t* strategy, ux_time_t dateTime, void* data);
    void (*on_exchange_reminder)(ux_strategy_t* strategy, ux_time_t dateTime, void* data);

    /* provider process */
    void (*on_provider_connected)(ux_strategy_t* strategy, ux_provider_t* provider);
    void (*on_provider_disconnected)(ux_strategy_t* strategy, ux_provider_t* provider);
    void (*on_provider_error)(ux_strategy_t* strategy, uxe_provider_error_t* error);

    /* market data process */
    void (*on_bid)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_bid_t* bid);
    void (*on_ask)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_ask_t* ask);
    void (*on_trade)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_trade_t* trade);
    void (*on_level2_snapshot)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_level2_snapshot_t* snapshot);
    void (*on_Level2_update)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_level2_update_t* update);
    void (*on_bar_open)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_bar_t* bar);
    void (*on_bar)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_bar_t* bar);
    void (*on_bar_slice)(ux_strategy_t* strategy, uxe_bar_slice_t* slice);
    void (*on_news)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_news_t* news);
    void (*on_fundamental)(ux_strategy_t* strategy, ux_instrument_t* instrument, uxe_fundamental_t* fundamental);

    /* execution process */
    void (*on_execution_report)(ux_strategy_t* strategy, uxe_execution_report_t* report);
    void (*on_account_report)(ux_strategy_t* strategy, uxe_account_report_t* report);
    void (*on_account_data)(ux_strategy_t* strategy, uxe_account_data_t *data);

    /* order  process */
    void (*on_send_order)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_pending_new_order)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_new_order)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_status_changed)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_filled)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_partially_filled)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_cancelled)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_replaced)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_rejected)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_expired)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_cancel_rejected)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_replace_rejected)(ux_strategy_t* strategy, ux_order_t* order);
    void (*on_order_done)(ux_strategy_t* strategy, ux_order_t* order);

    void (*on_fill)(ux_strategy_t* strategy, ux_fill_t* fill);
    void (*on_transaction)(ux_strategy_t* strategy, ux_transaction_t* transaction);

    /* portfolio process */
    void (*on_portfolio_added)(ux_strategy_t* strategy, ux_portfolio_t *portfolio);
    void (*on_portfolio_removed)(ux_strategy_t* strategy, ux_portfolio_t *portfolio);
    void (*on_portfolio_parent_changed)(ux_strategy_t* strategy, ux_portfolio_t *portfolio);

    /* position process */
    void (*on_position_opened)(ux_strategy_t* strategy, ux_position_t* position);
    void (*on_position_closed)(ux_strategy_t* strategy, ux_position_t* position);
    void (*on_position_changed)(ux_strategy_t* strategy, ux_position_t* position);

    /* stop process */
    void (*on_stop_executed)(ux_strategy_t* strategy, ux_stop_t* stop);
    void (*on_stop_cancelled)(ux_strategy_t* strategy, ux_stop_t* stop);
    void (*on_stop_status_changed)(ux_strategy_t* strategy, ux_stop_t* stop);

    /* misc */
    void (*on_command)(ux_strategy_t* strategy, uxe_command_t* command);
    void (*on_user_command)(ux_strategy_t* strategy, const char* command);
    void (*on_user_event)(ux_strategy_t* strategy, ux_event_t* e);
    /*
    void (*on_parameter_changed)(ux_strategy_t* strategy, Parameter oldParameter, Parameter newParameter);
    void (*on_property_changed)(ux_strategy_t* strategy, OnPropertyChanged onPropertyChanged);
    */
};

#ifdef __cplusplus
}
#endif

#endif // __UX_STRATEGY_H__
