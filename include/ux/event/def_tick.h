/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_DEF_TICK_H__
#define __UX_DEF_TICK_H__

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

#define UXE_TRADE_FIELDS                        \
    UXE_TICK_COMMON_FIELDS                      \
    int8_t direction;

typedef struct {
    double price;
    long size;
}ux_tick_info_t;

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

/* reminder event */

#define UXE_REMINDER_FIELDS                 \
    ux_clock_type clock_type;               \
    ux_reminder_cb callback;                \
    void *user_data;                        \
    ux_time_t timeout;                      \
    uint64_t repeat;                        \
    /*private*/                             \
    void *ctx;                              \
    void* heap_node[3];                     \
    uint64_t start_id;


#endif // __UX_DEF_TICK_H__
