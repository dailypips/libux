/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __INDICATOR_H__
#define __INDICATOR_H__

#include <ux/base/ux_type.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UX_INDEX_OPTION_NULL,
    UX_INDEX_OPTION_NEXT,
    UX_INDEX_OPTION_PREV
}ux_index_option;

struct iseries_t {
    char *name;
    char *desc;
    int (*get_count)(iseries_t *);
    ux_vec_t* (*get_indicators)(iseries_t *);
    double (*get_first)();
    double (*get_last)();
    double (*get)(int index);
    double (*get_bar)(int index, ux_bar_data_t data);
    int (*get_index)(ux_time_t time, ux_index_option option);
    ux_time_t (*get_time)(int index);
    double (*get_min)(ux_time_t start, ux_time_t end);
    double (*get_max)(ux_time_t start, ux_time_t end);
};
// MultiSeries, TickSeries, TimeSeries

struct indicator_s {
    ux_time_series_t *input;
    int calculate:1;
    int auto_update:1;
    void (*calculate)();
};

void indicator_attach(iseries_t *series);
void indicator_detach(iseries_t *series);


#ifdef __cplusplus
}
#endif

#endif // __INDICATOR_H__
