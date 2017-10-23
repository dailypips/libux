/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_account.h>

#ifdef __cplusplus
extern "C" {
#endif

/*typedef enum {
    UX_ACCOUNT_DATA_TYPE_ACCTVALUE,
    UX_ACCOUNT_DATA_TYPE_POSITION,
    UX_ACCOUNT_DATA_TYPE_ORDER
} ux_account_data_type;*/

#define PREDEFINE_ACCOUNT_FIELDS(_)  \
    /* CONSTANTS       | NAME */     \
    _(SYMBOL, "Symbol")              \
    _(EXCHANGE, "Exchange")          \
    _(SECURITY_TYPE, "SecurityType") \
    _(CURRENCY, "Currency")          \
    _(MATURITY, "Maturity")          \
    _(PUT_OR_CALL, "PutOrCall")      \
    _(STRIKE, "Strike")              \
    _(QTY, "Qty")                    \
    _(LONG_QTY, "LongQty")           \
    _(SHORT_QTY, "ShortQty")         \
    _(ORDER_ID, "OrderID")           \
    _(ORDER_TYPE, "OrderType")       \
    _(ORDER_SIDE, "OrderSide")       \
    _(ORDER_STATUS, "OrderStatus")   \
    _(PRICE, "Price")                \
    _(STOP_PX, "StopPx")

typedef struct ux_account_position_s {
    ux_currency_t currency;
    double value;
}ux_account_position_t;

typedef struct ux_account_transaction_s {
    ux_time_t timestamp;
    ux_currency_t currency;
    double value;
    char *text;
}ux_account_transaction_t;

struct ux_account_s {
    ux_currency_t currency;
    void* positions[2];
    void* transactions[2];
    struct ux_account_s *parent;
    int update_parent;
};

void ux_account_init(ux_account_t *account);
void ux_account_destroy(ux_account_t *account);

double ux_account_get_value(ux_account_t *account);
double ux_account_get_value_by_currency(ux_account_t *account);
ux_account_position_t* ux_account_get_position_by_currency(ux_account_t* account, ux_currency_t currency);

void ux_account_add_report(ux_account_t *account, uxe_account_report_t *report);
void ux_account_add_transaction(ux_account_t *account, ux_account_transaction_t *transaction, int update_parent);
void ux_account_add_fill(ux_account_t *account, ux_fill_t *fill, int update_parent);


#ifdef __cplusplus
}
#endif

#endif // __ACCOUNT_H__
