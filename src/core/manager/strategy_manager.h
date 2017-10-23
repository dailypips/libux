/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __STRATEGY_MANAGER_H__
#define __STRATEGY_MANAGER_H__

#include <ux/base/ux_type.h>
#include <ux/event/tick.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STRATEGY_MANAGER_FIELDS                     \
    ux_strategy_mode strategy_mode;                 \
    ux_strategy_status strategy_status;             \
    ux_strategy_persistence strategy_persistence;   \
    int next_id;                                    \
    ux_strategy_t *strategy;

#define STRATEGY_MANAGER_INIT(ctx) strategy_manager_init((ctx))
#define STRATEGY_MANAGER_DESTROY(ctx) strategy_manager_destroy((ctx))

UX_FUNC void strategy_manager_init(ux_ctx_t *ctx);
UX_FUNC void strategy_manager_destroy(ux_ctx_t *ctx);

ux_strategy_t *ux_get_strategy_by_id(ux_ctx_t *ctx, int id);
void ux_add_strategy_by_id(ux_ctx_t *ctx, int id, ux_strategy_t *streategy);


void strategy_manager_on_strategy_event(ux_ctx_t *ctx, void *data);
void strategy_manager_on_exception(ux_ctx_t *ctx, const char *source, ux_event_t *e, uxe_exception_t *exception);
void strategy_manager_on_instrument_added(ux_ctx_t *ctx, ux_instrument_t *instrument);
void strategy_manager_on_instrument_removed(ux_ctx_t *ctx, ux_instrument_t *instrument);
void strategy_manager_on_reminder(ux_ctx_t *ctx, ux_time_t dateTime, void *data);
void strategy_manager_on_exchange_reminder(ux_ctx_t *ctx, ux_time_t dateTime, void *data);
void strategy_manager_on_provider_connected(ux_ctx_t *ctx, ux_provider_t *provider);
void strategy_manager_on_provider_disconnected(ux_ctx_t *ctx, ux_provider_t *provider);
void strategy_manager_on_provider_error(ux_ctx_t *ctx, uxe_provider_error_t *error);
void strategy_manager_on_bid(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_bid_t *bid);
void strategy_manager_on_ask(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_ask_t *ask);
void strategy_manager_on_trade(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_trade_t *trade);
void strategy_manager_on_level2_snapshot(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_level2_snapshot_t *snapshot);
void strategy_manager_on_Level2_update(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_level2_update_t *update);
void strategy_manager_on_bar_open(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_bar_t *bar);
void strategy_manager_on_bar(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_bar_t *bar);
void strategy_manager_on_bar_slice(ux_ctx_t *ctx, uxe_bar_slice_t *slice);
void strategy_manager_on_news(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_news_t *news);
void strategy_manager_on_fundamental(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_fundamental_t *fundamental);
void strategy_manager_on_execution_report(ux_ctx_t *ctx, uxe_execution_report_t *report);
void strategy_manager_on_account_report(ux_ctx_t *ctx, uxe_account_report_t *report);
void strategy_manager_on_account_data(ux_ctx_t *ctx,  uxe_account_data_t *accountData);
void strategy_manager_on_send_order(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_pending_new_order(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_new_order(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_status_changed(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_filled(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_partially_filled(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_cancelled(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_replaced(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_rejected(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_expired(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_cancel_rejected(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_replace_rejected(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_order_done(ux_ctx_t *ctx, ux_order_t *order);
void strategy_manager_on_fill(ux_ctx_t *ctx, ux_fill_t *fill);
void strategy_manager_on_transaction(ux_ctx_t *ctx, ux_transaction_t *transaction);
void strategy_manager_on_position_opened(ux_ctx_t *ctx, ux_position_t *position);
void strategy_manager_on_position_closed(ux_ctx_t *ctx, ux_position_t *position);
void strategy_manager_on_position_changed(ux_ctx_t *ctx, ux_position_t *position);
void strategy_manager_on_stop_executed(ux_ctx_t *ctx, ux_stop_t *stop);
void strategy_manager_on_stop_cancelled(ux_ctx_t *ctx, ux_stop_t *stop);
void strategy_manager_on_stop_status_changed(ux_ctx_t *ctx, ux_stop_t *stop);
void strategy_manager_on_command(ux_ctx_t *ctx, uxe_command_t *command);
void strategy_manager_on_user_command(ux_ctx_t *ctx, const char *command);
void strategy_manager_on_user_event(ux_ctx_t *ctx, ux_event_t *e);

#ifdef __cplusplus
}
#endif

#endif // __STRATEGY_MANAGER_H__
