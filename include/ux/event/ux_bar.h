/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef BAR_H
#define BAR_H

static inline double ux_bar_get_range(uxe_bar_t *bar)
{
    return bar->high - bar->low;
}

static inline double ux_bar_get_median(uxe_bar_t *bar)
{
    return (bar->high + bar->low) / 2.0;
}

static inline double ux_bar_get_typical(uxe_bar_t *bar)
{
    return (bar->high + bar->low + bar->close) / 3.0;
}

static inline double ux_bar_get_weighted(uxe_bar_t *bar)
{
    return (bar->high + bar->low + bar->close * 2.0) / 4.0;
}

static inline double ux_bar_get_average(uxe_bar_t *bar)
{
    return (bar->high + bar->low + bar->open+ bar->close) / 4.0;
}

#endif // BAR_H
