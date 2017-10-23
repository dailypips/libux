/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_SERIES_H__
#define __UX_SERIES_H__

#include <ux/base/ux_type.h>
#include <ux/event/tick.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ux_tick_series_s {
    char *name;
    char *description;
    int count;
    ux_event_tick_t* ticks;
    ux_event_tick_t* min;
    ux_event_tick_t* max;
};

struct ux_bar_series_s {
    int id;
    char *name;
    int count;
    uxe_bar_t *bars;
    uxe_bar_t *min;
    uxe_bar_t *max;
    void* indicators[2];
};

struct ux_fill_series_s {
    char* name;
    int count;
    ux_fill_t* min;
    ux_fill_t* max;
    ux_fill_t* fills;
};

typedef struct ux_time_series_item_s {
    ux_time_t timestamp;
    double value;
} ux_time_series_item_t;

struct ux_time_series_s {
    int id;
    char* name;
    char* description;
    ux_time_series_item_t* min;
    ux_time_series_item_t* max;
    void* indicators[2];
    int count;
    ux_time_series_item_t* series;
};

#ifdef __cplusplus
}
#endif

#endif // __UX_SERIES_H__
