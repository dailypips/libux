/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UXE_MARKET_DATA_H__
#define __UXE_MARKET_DATA_H__

/* market data event */
#define UXE_MARKET_COMMON_FIELDS                \
    int provider;                               \
    int instrument;

#define UXE_MARKET_COMMON_EXCHANGE_FIELDS       \
    UXE_MARKET_COMMON_FIELDS                    \
    ux_time_t exchange_timestamp;

#define UXE_TICK_COMMON_FIELDS                  \
    UXE_MARKET_COMMON_EXCHANGE_FIELDS           \
    double price;                               \
    long size;

#define UXE_ASK_FIELDS   UXE_TICK_COMMON_FIELDS
#define UXE_BID_FIELDS   UXE_TICK_COMMON_FIELDS
#define UXE_TRADE_FIELDS UXE_TICK_COMMON_FIELDS

#define UXE_NEWS_FIELDS                         \
    UXE_MARKET_COMMON_FIELDS                    \
    char *urgency;                              \
    char *url;                                  \
    char *headline;                             \
    char *text;

#define UXE_FUNDAMENTAL_FIELDS                  \
    UXE_MARKET_COMMON_FIELDS

typedef struct {
    double price;
    long size;
}ux_tick_info_t;

#define UXE_LEVEL2_SNAPSHOT_FIELDS              \
    UXE_MARKET_COMMON_EXCHANGE_FIELDS           \
    size_t num_bids;                            \
    size_t num_asks;                            \
    ux_tick_info_t* ticks;

#define UXE_LEVEL2_UPDATE_FIELDS                \
    UXE_MARKET_COMMON_EXCHANGE_FIELDS           \
    size_t num_entries;                         \
    ux_level2_t *entries;


#define UXE_BARSLICE_FIELDS                     \
    long bar_size;

#define UXE_BAR_FIELDS                          \
    UXE_MARKET_COMMON_EXCHANGE_FIELDS           \
    ux_bar_type bar_type;                       \
    long size;                                  \
    ux_time_t open_time;                        \
    double high;                                \
    double low;                                 \
    double open;                                \
    double close;                               \
    long volume;                                \
    long open_int;                              \
    long tick_count;                            \
    ux_bar_status status;

#define UXE_BAR_OPEN_FIELDS UXE_BAR_FIELDS


/*static double ux_bar_get_range(uxe_bar_t *bar)
{
    return bar->high - bar->low;
}

static double ux_bar_get_median(uxe_bar_t *bar)
{
    return (bar->high + bar->low) / 2.0;
}

static double ux_bar_get_typical(uxe_bar_t *bar)
{
    return (bar->high + bar->low + bar->close) / 3.0;
}

static double ux_bar_get_weighted(uxe_bar_t *bar)
{
    return (bar->high + bar->low + bar->close * 2.0) / 4.0;
}

static double ux_bar_get_average(uxe_bar_t *bar)
{
    return (bar->high + bar->low + bar->open+ bar->close) / 4.0;
}*/

#endif // __UXE_MARKET_DATA_H__
