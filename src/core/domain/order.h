/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __ORDER_H__
#define __ORDER_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_order.h>

#ifdef __cplusplus
extern "C" {
#endif


UX_EXTERN void ux_order_on_execution_report(ux_order_t *order, uxe_execution_report_t *report);
UX_EXTERN void ux_order_on_execution_command(ux_order_t *order, uxe_execution_command_t *command);

void on_account_report();
void on_execution_report();

void delete_by_series_name(char *series_name);

void load(char *name /*null*/, int clientId /* -1 */);

#ifdef __cplusplus
}
#endif

#endif // __ORDER_H__
