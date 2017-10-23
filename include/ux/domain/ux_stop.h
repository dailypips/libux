/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_STOP_H__
#define __UX_STOP_H__

#include <ux/base/ux_type.h>
#include <ux/event/tick.h>
#include <ux/domain/ux_strategy.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UX_STOP_FILL_MODE_CLOSE,
    UX_STOP_FILL_MODE_HIGHLOW,
    UX_STOP_FILL_MODE_STOP
}ux_stop_fill_mode;

typedef enum {
    UX_STOP_MODE_ABSOLUTE,
    UX_STOP_MODE_PERCENT
}ux_stop_mode;

typedef enum {
    UX_STOP_STATUS_ACTIVE,
    UX_STOP_STATUS_EXECUTED,
    UX_STOP_STATUS_CANCELED
}ux_stop_status;

typedef enum {
    UX_STOP_TYPE_FIXED,
    UX_STOP_TYPE_TRAILING,
    UX_STOP_TYPE_TIME
}ux_stop_type;

typedef struct ux_stop_s {
    ux_strategy_t *strategy;
    ux_position_t *position;
    ux_instrument_t *instrument;
    int connected;
    ux_stop_type type;
    ux_stop_mode mode;
    ux_stop_status status;
    double level;
    double init_price;
    double current_price;
    double stop_price;
    double fill_price;
    double trail_price;
    double qty;
    ux_position_side side;
    ux_time_t creation_time;
    ux_time_t completion_time;
    int trace_on_quote :1;
    int trace_on_trade:1;
    int trace_on_bar:1;
    int trace_on_bar_open:1;
    int trail_on_open:1;
    int trail_on_highlow:1;
    long filter_bar_size;
    ux_bar_type filter_bar_type;
    ux_stop_fill_mode fill_mode;
}ux_stop_t;

#ifdef __cplusplus
}
#endif

#endif // __UX_STOP_H__
