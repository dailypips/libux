/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_POSITION_H__
#define __UX_POSITION_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_series.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UX_POSITION_SIDE_LONG,
    UX_POSITION_SIDE_SHORT
}ux_position_side;


struct ux_position_s {
    void *queue_node[2];
    void *portfolio;
    ux_instrument_t *instrument;
    ux_position_side side;
    ux_id_t portfolio_id;
    ux_iid_t instrument_id;
    double amount;
    double qty_bought;
    double qty_sold;
    double open_value;
    double avg_price;
    double pnl;
    void* fills[2];
    ux_fill_t *entry;
};

#ifdef __cplusplus
}
#endif

#endif // __UX_POSITION_H__
