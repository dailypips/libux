/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_DEF_EXECUTION_H__
#define __UX_DEF_EXECUTION_H__

#ifdef __cplusplus
extern "C" {
#endif

/* execution */
#define UXE_EXECUTION_COMMON_FIELDS                                            \
  int provider;                                                                \
  int instrument_id;                                                           \
  ux_instrument_t *instrument;                                                 \
  int id;                                                                      \
  ux_order_t *order;                                                           \
  int order_id;                                                                \
  char *client_order_id;                                                       \
  char *provider_order_id;                                                     \
  int client_id;                                                               \
  int is_loadded;                                                              \
  int provider_id;                                                             \
  ux_execution_provider_t *execution_provider;                                 \
  ux_currency_t currency;

#define UXE_ACCOUNT_REPORT_FIELDS                                              \
  UXE_EXECUTION_COMMON_FIELDS                                                  \
  int portfolio;                                                               \
  double amount;                                                               \
  char *text;

#define UXE_EXECUTION_REPORT_FIELDS                                            \
  UXE_EXECUTION_COMMON_FIELDS                                                  \
  int route_id;                                                                \
  int algo_id;                                                                 \
  int portfolio_id;                                                            \
  ux_portfolio_t *portfolio;                                                   \
  int strategy_id;                                                             \
  ux_time_t transact_time;                                                     \
  double min_qty;                                                              \
  double peg_difference;                                                       \
  char *exec_inst;                                                             \
  char *oca;                                                                   \
  char *account;                                                               \
  char *clientID;                                                              \
  ux_exec_type exec_type;                                                      \
  ux_order_type order_type;                                                    \
  ux_order_side order_side;                                                    \
  ux_tif tif;                                                                  \
  ux_time_t expire_time;                                                       \
  ux_order_status order_status;                                                \
  double last_price;                                                           \
  double avg_price;                                                            \
  double order_qty;                                                            \
  double cum_qty;                                                              \
  double last_qty;                                                             \
  double leaves_qty;                                                           \
  double price;                                                                \
  double stop_price;                                                           \
  double commission;                                                           \
  char *text;                                                                  \
  char *exec_id;

typedef enum {
  UX_EXECUTION_COMMAND_SEND,
  UX_EXECUTION_COMMAND_CANCEL,
  UX_EXECUTION_COMMAND_REPLACE
} ux_execution_command_type;

#define UXE_EXECUTION_COMMAND_FIELDS                                           \
  UXE_EXECUTION_COMMON_FIELDS                                                  \
  int route_id;                                                                \
  ux_execution_command_type ec_type;                                           \
  char *symbol;                                                                \
  int algo;                                                                    \
  ux_portfolio_t portfolio;                                                    \
  int portfolio_id;                                                            \
  int strategy_id;                                                             \
  ux_time_t transact_time;                                                     \
  ux_order_side order_side;                                                    \
  ux_order_type order_type;                                                    \
  ux_tif tif;                                                                  \
  double price;                                                                \
  double stop_price;                                                           \
  double qty;                                                                  \
  double min_qty;                                                              \
  double peg_difference;                                                       \
  char *exec_inst;                                                             \
  char *oca;                                                                   \
  char *text;                                                                  \
  char *account;                                                               \
  char *clientID;

typedef struct ux_account_data_fields_s {
  // TODO
} ux_account_data_fields_t;

typedef enum {
    UX_ACCOUNT_DATA_TYPE_VALUE,
    UX_ACCOUNT_DATA_TYPE_POSITION,
    UX_ACCOUNT_DATA_TYPE_ORDER
}ux_account_data_type;

#define UXE_ACCOUNT_DATA_FIELDS                                                \
  ux_account_data_type account_data_type;                                      \
  char *account;                                                               \
  int provider_id;                                                             \
  int route_id;                                                                \
  ux_account_data_fields_t fields;

struct uxe_on_fill_s {
  ux_fill_t *fill;
  ux_portfolio_t *portfolio;
};

struct uxe_on_transaction_s {
  ux_transaction_t *transaction;
  ux_portfolio_t *portfolio;
};

#define UXE_ORDER_COMMON_FIELDS ux_order_t *order;

#define UXE_ORDER_SENT_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_PENDING_NEW_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_STATUS_CHANGED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_NEW_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_PARTIALLY_FIILED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_FILLED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_REPLACED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_CANCELLED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_REJECTED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_EXPIRED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_CANCEL_REJECTED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_REPLACE_REJECTED_FIELDS UXE_ORDER_COMMON_FIELDS

#define UXE_ORDER_DONE_FIELDS UXE_ORDER_COMMON_FIELDS

#ifdef __cplusplus
}
#endif

#endif // __UX_DEF_EXECUTION_H__
