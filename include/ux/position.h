/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_POSITION_H__
#define __UX_POSITION_H__

#include <ux/common.h>
#include <ux/types.h>
#include <ux/enums.h>
#include <ux/memory.h>
#include <ux/datetime.h>
#include <ux/currency.h>
#include <ux/order.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ux_fill_s {
    ux_order_t *order;
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

struct ux_transaction_s {
    double qty;
    double price;
    double commission;
    double is_done;
    void*  fills[2];
};

typedef enum {
    UX_POSITION_SIDE_LOG,
    UX_POSITION_SIDE_SHORT
}ux_position_side;


struct ux_position_s {
    ux_portfolio_t *portfolio;
    ux_instrument_t *instrument;
    int portfolio_id;
    int instrument_id;
    double amount;
    double qty_bought;
    double qty_sold;
    double open_value;
    double avg_price;
    double pnl;
    void* fills[2];
    ux_fill_t entry;
};

#ifdef __cplusplus
}
#endif

#endif // __UX_POSITION_H__
