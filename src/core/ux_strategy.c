/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright )(ux_strategy_t *strategy, C); 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"




/* strategy */
void (*on_strategy_init)(ux_strategy_t *strategy, );
void (*on_strategy_start)(ux_strategy_t *strategy, );
void (*on_strategy_stop)(ux_strategy_t *strategy, );
void (*on_strategy_event)(ux_strategy_t *strategy, void *data);
void (*on_exception)(ux_strategy_t *strategy, const char *source, ux_event_t *e, uxe_exception_t *exception);
void (*on_instrument_added)(ux_strategy_t *strategy, ux_instrument_t *instrument);
void (*on_instrument_removed)(ux_strategy_t *strategy, ux_instrument_t *instrument);
void (*on_reminder)(ux_strategy_t *strategy, ux_time_t dateTime, void *data);
void (*on_exchange_reminder)(ux_strategy_t *strategy, ux_time_t dateTime, void *data);
void (*on_provider_connected)(ux_strategy_t *strategy, ux_provider_t *provider);
void (*on_provider_disconnected)(ux_strategy_t *strategy, ux_provider_t *provider);
void (*on_provider_error)(ux_strategy_t *strategy, ux_provider_error_t *error);
void (*on_bid)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_bid_t *bid);
void (*on_ask)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_ask_t *ask);
void (*on_trade)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_trade_t *trade);
void (*on_level2_snapshot)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_level2_snapshot_t *snapshot);
void (*on_Level2_update)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_level2_update_t *update);
void (*on_bar_open)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_bar_t *bar);
void (*on_bar)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_bar_t *bar);
void (*on_bar_slice)(ux_strategy_t *strategy, uxe_bar_slice_t *slice);
void (*on_news)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_news_t *news);
void (*on_fundamental)(ux_strategy_t *strategy, ux_instrument_t *instrument, uxe_fundamental *fundamental);
void (*on_execution_report)(ux_strategy_t *strategy, uxe_execution_report_t *report);
void (*on_account_report)(ux_strategy_t *strategy, uxe_account_report_t *report);
void (*on_send_order)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_pending_new_order)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_new_order)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_status_changed)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_filled)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_partially_filled)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_cancelled)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_replaced)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_rejected)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_expired)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_cancel_rejected)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_replace_rejected)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_order_done)(ux_strategy_t *strategy, ux_order_t *order);
void (*on_fill)(ux_strategy_t *strategy, ux_fill_t *fill);
void (*on_transaction)(ux_strategy_t *strategy, ux_transaction_t *transaction);
void (*on_position_opened)(ux_strategy_t *strategy, ux_position_t *position);
void (*on_position_closed)(ux_strategy_t *strategy, ux_position_t *position);
void (*on_position_changed)(ux_strategy_t *strategy, ux_position_t *position);
void (*on_stop_executed)(ux_strategy_t *strategy, ux_stop_t *stop);
void (*on_stop_cancelled)(ux_strategy_t *strategy, ux_stop_t *stop);
void (*on_stop_status_changed)(ux_strategy_t *strategy, ux_stop_t *stop);
void (*on_command)(ux_strategy_t *strategy, ux_command_t *command);
void (*on_user_command)(ux_strategy_t *strategy, const char *command);
void (*on_user_event)(ux_strategy_t *strategy, ux_event_t *e);
void (*on_parameter_changed)(ux_strategy_t *strategy, Parameter oldParameter, Parameter newParameter);
void (*on_account_data)(ux_strategy_t *strategy, AccountData accountData);
void (*on_property_changed)(ux_strategy_t *strategy, OnPropertyChanged onPropertyChanged);
