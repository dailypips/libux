/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef SERIES_H
#define SERIES_H

#include <ux/base/ux_common.h>
#include <ux/base/ux_types.h>
#include <ux/base/ux_currency.h>
#include <ux/event/ux_event.h>
#include <ux/domain/ux_position.h>
#include <ux/domain/ux_provider.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ux_tick_series_s {
    char *name;
    char *description;
    int count;
    ux_event_tick_t* ticks;
    ux_event_tick_t* min;
    ux_event_tick_t* max;
}ux_tick_series_t;

typedef struct ux_bar_series_s {
    int id;
    char *name;
    int count;
    uxe_bar_t *bars;
    uxe_bar_t *min;
    uxe_bar_t *max;
    void* indicators[2];
}ux_bar_series_t;

#ifdef __cplusplus
}
#endif

#endif // SERIES_H
