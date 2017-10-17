#ifndef __UXE_EVENTS_H__
#define __UXE_EVENTS_H__

#include <ux/common.h>
#include <ux/types.h>
#include <ux/datetime.h>
#include <ux/currency.h>
#include <ux/event.h>
#include <ux/enums.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ux/market_def.h>
#include <ux/execution_def.h>
#include <ux/system_def.h>

struct uxe_reminder_s {
  UXE_COMMON_FIELDS
  UXE_REMINDER_FIELDS
};

struct ux_event_tick_s {
    UXE_COMMON_FIELDS
    UXE_TICK_COMMON_FIELDS
};

struct uxe_ask_s {
  UXE_COMMON_FIELDS
  UXE_ASK_FIELDS
};

struct uxe_bid_s {
  UXE_COMMON_FIELDS
  UXE_BID_FIELDS
};

struct uxe_trade_s {
  UXE_COMMON_FIELDS
  UXE_TRADE_FIELDS
};

struct uxe_level2_snapshot_s {
  UXE_COMMON_FIELDS
  UXE_LEVEL2_SNAPSHOT_FIELDS
};

struct uxe_level2_update_s {
  UXE_COMMON_FIELDS
  UXE_LEVEL2_UPDATE_FIELDS
};

struct uxe_bar_s {
  UXE_COMMON_FIELDS
  UXE_BAR_FIELDS
};

struct uxe_bar_open_s {
  UXE_COMMON_FIELDS
  UXE_BAR_OPEN_FIELDS
};

struct uxe_bar_slice_s {
  UXE_COMMON_FIELDS
  UXE_BARSLICE_FIELDS
};

struct uxe_news_s {
  UXE_COMMON_FIELDS
  UXE_NEWS_FIELDS
};

struct uxe_fundamental_s {
  UXE_COMMON_FIELDS
  UXE_FUNDAMENTAL_FIELDS
};

struct uxe_instrument_added_s {
  UXE_COMMON_FIELDS
  UXE_INSTRUMENT_ADDED_FIELDS
};

struct uxe_instrument_removed_s {
  UXE_COMMON_FIELDS
  UXE_INSTRUMENT_REMOVED_FIELDS
};

struct uxe_historical_data_s {
  UXE_COMMON_FIELDS
  UXE_HISTORICAL_DATA_FIELDS
};

struct uxe_historical_data_end_s {
  UXE_COMMON_FIELDS
  UXE_HISTORICAL_DATA_END_FIELDS
};

struct uxe_account_report_s {
  UXE_COMMON_FIELDS
  UXE_ACCOUNT_REPORT_FIELDS
};

struct uxe_execution_report_s {
  UXE_COMMON_FIELDS
  UXE_EXECUTION_REPORT_FIELDS
};

struct uxe_execution_command_s {
  UXE_COMMON_FIELDS
  UXE_EXECUTION_COMMAND_FIELDS
};

struct uxe_order_status_changed_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_STATUS_CHANGED_FIELDS
};

struct uxe_order_filled_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_FILLED_FIELDS
};

struct uxe_order_pending_new_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_PENDING_NEW_FIELDS
};

struct uxe_order_new_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_NEW_FIELDS
};
struct uxe_order_partially_filled_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_PARTIALLY_FIILED_FIELDS
};

struct uxe_order_cancelled_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_CANCELLED_FIELDS
};

struct uxe_order_replaced_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_REPLACED_FIELDS
};

struct uxe_order_rejected_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_REJECTED_FIELDS
};

struct uxe_order_expired_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_EXPIRED_FIELDS
};

struct uxe_order_cancel_rejected_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_CANCEL_REJECTED_FIELDS
};

struct uxe_order_replace_rejected_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_REPLACE_REJECTED_FIELDS
};

struct uxe_order_done_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_DONE_FIELDS
};

struct uxe_order_sent_s {
  UXE_COMMON_FIELDS
  UXE_ORDER_SENT_FIELDS
};

struct uxe_portfolio_added_s {
  UXE_COMMON_FIELDS
  UXE_PORTFOLIO_ADDED_FIELDS
};

struct uxe_portfolio_removed_s {
  UXE_COMMON_FIELDS
  UXE_PORTFOLIO_REMOVED_FIELDS
};

struct uxe_portfolio_parent_changed_s {
  UXE_COMMON_FIELDS
  UXE_PORTFOLIO_PARENT_CHANGED_FIELDS
};

struct uxe_portfolio_update_s {
  UXE_COMMON_FIELDS
  UXE_PORTFOLIO_UPDATE_FIELDS
};

struct uxe_position_opened_s {
  UXE_COMMON_FIELDS
  UXE_POSITION_OPENED_FIELDS
};

struct uxe_position_closed_s {
  UXE_COMMON_FIELDS
  UXE_POSITION_CLOSED_FIELDS
};

struct uxe_position_changed_s {
  UXE_COMMON_FIELDS
  UXE_POSITION_CHANGED_FIELDS
};

struct uxe_stop_executed_s {
  UXE_COMMON_FIELDS
  UXE_STOP_EXECUTED_FIELDS
};

struct uxe_stop_cancelled_s {
  UXE_COMMON_FIELDS
  UXE_STOP_CANCELLED_FIELDS
};

struct uxe_stop_status_changed_s {
  UXE_COMMON_FIELDS
  UXE_STOP_STATUS_CHANGED_FIELDS
};

struct uxe_command_s {
  UXE_COMMON_FIELDS
  UXE_COMMAND_FIELDS
};

struct uxe_user_command_s {
  UXE_COMMON_FIELDS
  UXE_USER_COMMAND_FIELDS
};

struct uxe_group_update_s {
  UXE_COMMON_FIELDS
  UXE_GROUP_UPDATE_FIELDS
};

struct uxe_group_event_s {
  UXE_COMMON_FIELDS
  UXE_GROUP_EVENT_FIELDS
};

struct uxe_queue_opened_s {
  UXE_COMMON_FIELDS
  UXE_QUEUE_OPENED_FIELDS
};

struct uxe_queue_closed_s {
  UXE_COMMON_FIELDS
  UXE_QUEUE_CLOSED_FIELDS
};

struct uxe_simulator_start_s {
  UXE_COMMON_FIELDS
  UXE_SIMULATOR_START_FIELDS
};

struct uxe_simulator_stop_s {
  UXE_COMMON_FIELDS
  UXE_SIMULATOR_STOP_FIELDS
};

struct uxe_simulator_progress_s {
  UXE_COMMON_FIELDS
  UXE_SIMULATOR_PROGRESS_FIELDS
};

struct uxe_provider_added_s {
  UXE_COMMON_FIELDS
  UXE_PROVIDER_ADDED_FIELDS
};

struct uxe_provider_removed_s {
  UXE_COMMON_FIELDS
  UXE_PROVIDER_REMOVED_FIELDS
};

struct uxe_provider_connected_s {
  UXE_COMMON_FIELDS
  UXE_PROVIDER_CONNECTED_FIELDS
};

struct uxe_provider_disconnected_s {
  UXE_COMMON_FIELDS
  UXE_PROVIDER_DISCONNECTED_FIELDS
};

struct uxe_provider_status_changed_s {
  UXE_COMMON_FIELDS
  UXE_PROVIDER_STATUS_CHANGED_FIELDS
};

struct uxe_provider_error_s {
  UXE_COMMON_FIELDS
  UXE_PROVIDER_ERROR_FIELDS
};

struct uxe_output_s {
  UXE_COMMON_FIELDS
  UXE_OUTPUT_FIELDS
};

struct uxe_group_s {
  UXE_COMMON_FIELDS
  UXE_GROUP_FIELDS
};

struct uxe_exception_s {
  UXE_COMMON_FIELDS
  UXE_EXCEPTION_FIELDS
};


#ifdef __cplusplus
}
#endif

#endif // __UXE_EVENTS_H__