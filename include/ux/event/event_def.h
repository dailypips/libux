/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_EVENT_DEF_H__
#define __UX_EVENT_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* event module */
#define EVENTDEF(_)                                                                              \
    /* UppercaseName        LowcaseName         destroy            clone              dispatch*/ \
    /* timer event */                                                                            \
    _(REMINDER, reminder, default, default, reminder)                                            \
    /* market events */                                                                          \
    _(ASK, ask, default, default, ask)                                                           \
    _(BID, bid, default, default, bid)                                                           \
    _(TRADE, trade, default, default, trade)                                                     \
    _(LEVEL2_SNAPSHOT, level2_snapshot, level2_snapshot, level2_snapshot, default)               \
    _(LEVEL2_UPDATE, level2_update, level2_update, level2_update, default)                       \
    _(BAR, bar, default, default, default)                                                       \
    _(BAR_OPEN, bar_open, default, default, default)                                             \
    _(BARSLICE, bar_slice, default, default, default)                                            \
    _(NEWS, news, news, news, default)                                                           \
    _(FUNDAMENTAL, fundamental, default, default, default)                                       \
    /* instrument */                                                                             \
    _(INSTRUMENT_DEF, instrument_def, default, default, default)                                 \
    _(INSTRUMENT_DEF_END, instrument_def_end, default, default, default)                         \
    _(INSTRUMENT_ADDED, instrument_added, default, default, default)                             \
    _(INSTRUMENT_REMOVED, instrument_removed, default, default, default)                         \
    /* historical event */                                                                       \
    _(HISTORICAL_DATA, historical_data, default, default, default)                               \
    _(HISTORICAL_DATA_END, historical_data_end, default, default, default)                       \
    /* execution events */                                                                       \
    _(ACCOUNT_DATA, account_data, default, default, default)                                     \
    _(ACCOUNT_REPORT, account_report, account_report, account_report, default)                   \
    _(EXECUTION_REPORT, execution_report, execution_report, execution_report, default)           \
    _(EXECUTION_COMMAND, execution_command, execution_command, execution_command, default)       \
    /* order event */                                                                            \
    _(ORDER_STATUS_CHANGED, order_status_changed, default, default, default)                     \
    _(ORDER_FILLED, order_filled, default, default, default)                                     \
    _(ORDER_PENDING_NEW, order_pending_new, default, default, default)                           \
    _(ORDER_NEW, order_new, default, default, default)                                           \
    _(ORDER_PARTIALLY_FIILED, order_partially_filled, default, default, default)                 \
    _(ORDER_CANCELLED, order_cancelled, default, default, default)                               \
    _(ORDER_REPLACED, order_replaced, default, default, default)                                 \
    _(ORDER_REJECTED, order_rejected, default, default, default)                                 \
    _(ORDER_EXPIRED, order_expired, default, default, default)                                   \
    _(ORDER_CANCEL_REJECTED, order_cancel_rejected, default, default, default)                   \
    _(ORDER_REPLACE_REJECTED, order_replace_rejected, default, default, default)                 \
    _(ORDER_DONE, order_done, default, default, default)                                         \
    _(ORDER_SENT, order_sent, default, default, default)                                         \
    /* portfolio event */                                                                        \
    _(PORTFOLIO_ADDED, portfolio_added, default, default, default)                               \
    _(PORTFOLIO_REMOVED, portfolio_removed, default, default, default)                           \
    _(PORTFOLIO_PARENT_CHANGED, portfolio_parent_changed, default, default, default)             \
    _(PORTFOLIO_UPDATE, portfolio_update, default, default, default)                             \
    /* position event */                                                                         \
    _(POSITION_OPENED, position_opened, default, default, default)                               \
    _(POSITION_CLOSED, position_closed, default, default, default)                               \
    _(POSITION_CHANGED, position_changed, default, default, default)                             \
    /* fill */                                                                                   \
    _(ON_FILL, on_fill, default, default, default)                                               \
    _(ON_TRANSACETION, on_transaction, default, default, default)                                \
    /* stop events */                                                                            \
    _(STOP_EXECUTED, stop_executed, default, default, default)                                   \
    _(STOP_CANCELLED, stop_cancelled, default, default, default)                                 \
    _(STOP_STATUS_CHANGED, stop_status_changed, default, default, default)                       \
    /* command event */                                                                          \
    _(COMMAND, command, default, default, default)                                               \
    _(USER_COMMAND, user_command, default, default, default)                                     \
    /* internal events */                                                                        \
    _(QUEUE_OPENED, queue_opened, default, default, default)                                     \
    _(QUEUE_CLOSED, queue_closed, default, default, default)                                     \
    /* */                                                                                        \
    _(SIMULATOR_START, simulator_start, default, default, default)                               \
    _(SIMULATOR_STOP, simulator_stop, default, default, default)                                 \
    _(SIMULATOR_PROGRESS, simulator_progress, default, default, default)                         \
    /* */                                                                                        \
    _(OPTIMIZATION_START, optimization_start, default, default, default)                         \
    _(OPTIMIZATION_STOP, optimization_stop, default, default, default)                           \
    _(OPTIMIZATION_PROGRESS, optimization_progress, default, default, default)                   \
    /* provider event */                                                                         \
    _(PROVIDER_ADDED, provider_added, default, default, default)                                 \
    _(PROVIDER_REMOVED, provider_removed, default, default, default)                             \
    _(PROVIDER_CONNECTED, provider_connected, default, default, default)                         \
    _(PROVIDER_DISCONNECTED, provider_disconnected, default, default, default)                   \
    _(PROVIDER_STATUS_CHANGED, provider_status_changed, default, default, default)               \
    _(PROVIDER_ERROR, provider_error, default, default, default)                                 \
    /* output event */                                                                           \
    _(OUTPUT, output, default, default, default)                                                 \
    /* group event */                                                                            \
    _(GROUP, group, default, default, default)                                                   \
    _(GROUP_UPDATE, group_update, default, default, default)                                     \
    _(GROUP_EVENT, group_event, default, default, default)                                       \
    /* exception */                                                                              \
    _(EXCEPTION, exception, exception, exception, default)

#ifdef __cplusplug
}
#endif

#endif // __UX_EVENT_DEF_H__
