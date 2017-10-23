/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UXE_MARKET_DATA_H__
#define __UXE_MARKET_DATA_H__

/* market data event */
typedef enum {
    UX_L2_ACTION_NEW,
    UX_L2_ACTION_CHANGE,
    UX_L2_ACTION_DELETE,
    UX_L2_ACTION_RESET
} ux_level2_action;

typedef enum {
    UX_L2SIDE_BID,
    UX_L2SIDE_ASK,
} ux_level2_side;

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

#endif // __UXE_MARKET_DATA_H__
