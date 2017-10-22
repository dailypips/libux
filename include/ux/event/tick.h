/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef TICK_H
#define TICK_H

#include <ux/event/ux_event.h>
#include <ux/event/def_tick.h>

typedef enum {
    UX_BAR_TYPE_NONE = 0,
    UX_BAR_TYPE_TIME,
    UX_BAR_TYPE_TICK,
    UX_BAR_TYPE_VOLUME,
    UX_BAR_TYPE_RANGE,
    UX_BAR_TYPE_SESSION,
    UX_BAR_TYPE_RENKO,
    UX_BAR_TYPE_CUSTOM
} ux_bar_type;

typedef enum {
    UX_BAR_STATUS_INCOMPLETE,
    UX_BAR_STATUS_COMPLETE,
    UX_BAR_STATUS_OPEN,
    UX_BAR_STATUS_HIGH,
    UX_BAR_STATUS_LOW,
    UX_BAR_STATUS_CLOSE
} ux_bar_status;

typedef enum {
    UX_BAR_INPUT_TRADE,
    UX_BAR_INPUT_BID,
    UX_BAR_INPUT_ASK,
    UX_BAR_INPUT_MIDDLE,
    UX_BAR_INPUT_TICK,
    UX_BAR_INPUT_BIDASK
} ux_bar_input;

struct ux_event_tick_s {
    UXE_COMMON_FIELDS
    UXE_TICK_COMMON_FIELDS
};

struct uxe_ask_s {
    UXE_COMMON_FIELDS
    UXE_ASK_FIELDS
};

struct uxe_bid_s {
    UXE_COMMON_FIELDS
    UXE_BID_FIELDS
};

struct uxe_trade_s {
    UXE_COMMON_FIELDS
    UXE_TRADE_FIELDS
};

struct uxe_bar_s {
    UXE_COMMON_FIELDS
    UXE_BAR_FIELDS
};

struct uxe_bar_open_s {
    UXE_COMMON_FIELDS
    UXE_BAR_OPEN_FIELDS
};

struct uxe_bar_slice_s {
    UXE_COMMON_FIELDS
    UXE_BARSLICE_FIELDS
};

typedef enum {
    UX_CLOCK_LOCAL  = 0,
    UX_CLOCK_EXCHANGE,
    UX_CLOCK_LAST
}ux_clock_type;

typedef void (*ux_reminder_cb)(uxe_reminder_t *timer);

struct uxe_reminder_s {
    UXE_COMMON_FIELDS
    UXE_REMINDER_FIELDS
};

#endif // TICK_H
