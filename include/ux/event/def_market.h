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
#define UXE_LEVEL2_SNAPSHOT_FIELDS              \
    UXE_MARKET_COMMON_EXCHANGE_FIELDS           \
    size_t num_bids;                            \
    size_t num_asks;                            \
    ux_tick_info_t* bids;                       \
    ux_tick_info_t* asks;

typedef struct {
    ux_level2_side side;
    ux_level2_action action;
    int position;
}ux_level2_t;

#define UXE_LEVEL2_UPDATE_FIELDS                \
    UXE_MARKET_COMMON_EXCHANGE_FIELDS           \
    size_t num_entries;                         \
    ux_level2_t *entries;

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

#endif // __UXE_MARKET_DATA_H__
