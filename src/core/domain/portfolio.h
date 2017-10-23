/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __PORTFOLIO_H__
#define __PORTFOLIO_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_portfolio.h>

#ifdef __cplusplus
extern "C" {
#endif

double ux_portfolio_get_active_orders_value(ux_portfolio_t *portfolio);
double ux_portfolio_get_position_value(ux_portfolio_t *portfolio);

ux_position_t *
ux_portfolio_get_position_by_instrument_id(ux_portfolio_t *portfolio,
                                           int instrument_id);
ux_transaction_t *
ux_portfolio_get_transaction_by_order_id(ux_portfolio_t *portfolio,
                                         int order_id);
void ux_portfolio_add_transaction(ux_portfolio_t *portfolio,
                                  ux_transaction_t *transacation, int order_id,
                                  int queued);
void ux_portfolio_add_fill(ux_portfolio_t *portfolio, ux_fill_t *fill,
                           int queued);
void ux_portfolio_on_execution_report(ux_portfolio_t *portfolio,
                                      uxe_execution_report_t *report,
                                      int queued);

#ifdef __cplusplus
}
#endif

#endif // __PORTFOLIO_H__

