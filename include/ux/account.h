/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_ACCOUNT_H__
#define __UX_ACCOUNT_H__

#include <ux/common.h>
#include <ux/types.h>
#include <ux/currency.h>

#ifdef __cplusplus
extern "C" {
#endif

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

double ux_account_get_value(ux_account_t *account);
double ux_account_get_value_by_currency(ux_account_t *account);
ux_account_position_t* ux_account_get_position_by_currency(ux_account_t* account, ux_currency_t currency);
void ux_account_add_report(ux_account_t *account, uxe_account_report_t *report);
void ux_account_add_transaction(ux_account_t *account, ux_account_transaction_t *transaction, int update_parent);
void ux_account_add_fill(ux_account_t *account, ux_fill_t *fill, int update_parent);

#ifdef __cplusplus
}
#endif

#endif // __UX_ACCOUNT_H__
