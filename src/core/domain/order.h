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

static inline int order_is_done(ux_order_t *order)
{
    return order->orderStatus == UX_ORDER_STATUS_FILLED ||
            order->orderStatus == UX_ORDER_STATUS_CANCELLED||
            order->orderStatus == UX_ORDER_STATUS_REJECTED ||
            order->orderStatus == UX_ORDER_STATUS_EXPIRED;
}

UX_FUNC void order_on_execution_report(ux_order_t *order, uxe_execution_report_t *report);
UX_FUNC void order_on_execution_command(ux_order_t *order, uxe_execution_command_t *command);


#ifdef __cplusplus
}
#endif

#endif // __ORDER_H__
