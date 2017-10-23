/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __FILL_H__
#define __FILL_H__

#include <ux/base/ux_type.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ux_fill_s {
    void *queue_node[2];
    ux_order_t *order;
    ux_instrument_t *instrument;
    int order_id;
    int portfolio_id;
    int instrument_id;
    ux_currency_t currency;
    ux_order_side side;
    double qty;
    double price;
    char *text;
    double commission;
};

#ifdef __cplusplus
}
#endif

#endif // __FILL_H__
