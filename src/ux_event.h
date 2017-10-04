/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_EVENT_H__
#define __UX_EVENT_H__

#include "ux_common.h"
#include "ux_atomic.h"

typedef enum {
  UX_EVENT_FLAG_NONE         = 1 << 0,
  UX_EVENT_FLAG_MARKET       = 1 << 1,
  UX_EVENT_FLAG_TICK         = 1 << 2,
  UX_EVENT_FLAG_EXECUTION    = 1 << 3,
  UX_EVENT_FLAG_SERVICE      = 1 << 4,
  UX_EVENT_FLAG_REMINDER     = 1 << 5
} ux_event_flag;


#define EVENTDEF(_) \
    /* UpperName | lowcaseName | init |  destory | clone | dispatch | flag */ \
    _(REMINDER, reminder, NULL, NULL, NULL, reminder, UX_EVENT_FLAG_REMINDER) \
    _(ASK,      ask,      NULL, NULL, NULL, ask, UX_EVENT_FLAG_MARKET | UX_EVENT_FLAG_TICK) \
    _(BID,      bid,      NULL, NULL, NULL, bid, UX_EVENT_FLAG_MARKET | UX_EVENT_FLAG_TICK) \
    _(TRADE,    trade,    NULL, NULL, NULL, trade, UX_EVENT_FLAG_MARKET | UX_EVENT_FLAG_TICK) \
    /*_(BAR,      bar,    NULL, NULL, NULL, NULL, UX_EVENT_FLAG_MARKET)*/ \
    _(NEWS,     news,     NULL,     event_news_destory, event_news_clone, default, UX_EVENT_FLAG_MARKET) \
    _(FUNDAMENTAL, fundamental, NULL, NULL, NULL, default, UX_EVENT_FLAG_MARKET) \
    /* internal event */ \
    _(QUEUE_OPENED, queue_opened, NULL, NULL, NULL, default, UX_EVENT_FLAG_NONE) \
    _(QUEUE_CLOSED, queue_closed, NULL, NULL, NULL, default, UX_EVENT_FLAG_NONE) \

typedef enum {
#define EVENTENUM(name, lname, init, destory, clone, processor, flag)	UX_EVENT_##name,
EVENTDEF(EVENTENUM)
#undef EVENTENUM
    UX_EVENT_LAST
} ux_event_type;

#define UX_EVENT_PUBLIC_FIELDS \
    ux_atomic_t refcount;        \
    ux_event_type type;        \
    ux_event_flag flag;        \
    datetime_t timestamp;       \

typedef struct ux_event_s {
  UX_EVENT_PUBLIC_FIELDS
}ux_event_t;

ux_event_t* ux_event_malloc(ux_event_type type);
ux_event_t* ux_event_ref(ux_event_t *e);
void ux_event_unref(ux_event_t *e);
ux_event_t* ux_event_clone(ux_event_t *e);
void ux_event_dispatch(void *loop, ux_event_t *e);

/* reminder */

typedef void (*reminder_cb)(datetime_t time, void *data);

typedef struct ux_event_reminder_s {
    UX_EVENT_PUBLIC_FIELDS
    ux_clock_type clock_type;
    reminder_cb callback;
    void *user_data;
    datetime_t start;
    datetime_t timeout;
    datetime_t repeat;
    /*private*/
    void *bus;
    void* heap_node[3];
    uint64_t start_id;
}ux_event_reminder_t;

/* market data event */
#define UX_MARKET_EVENT_PUBLIC_FIELD   \
    UX_EVENT_PUBLIC_FIELDS             \
    int provider;                       \
    int instrument;                     \

typedef struct tick_event_s {
    UX_MARKET_EVENT_PUBLIC_FIELD
    datetime_t exchange_timestamp;
    double price;
    long size;
} ux_event_tick_t;

typedef ux_event_tick_t ux_event_ask_t;
typedef ux_event_tick_t ux_event_bid_t;
typedef ux_event_tick_t ux_event_trade_t;


typedef struct {
    UX_MARKET_EVENT_PUBLIC_FIELD
    char *urgency;
    char *url;
    char *headline;
    char *text;
} ux_event_news_t;

typedef struct {
    UX_MARKET_EVENT_PUBLIC_FIELD
} ux_event_fundamental_t;

/* system */
typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    void *q;
}ux_event_queue_t;

typedef ux_event_queue_t ux_event_queue_opened_t;
typedef ux_event_queue_t ux_event_queue_closed_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_start_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_step_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_stop_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_resume_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_paused_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    datetime_t time1;
    datetime_t time2;
    long long count;
}ux_event_on_simulator_start_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_on_simulator_stop_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    char *source;
}ux_event_on_exception_t;

/* special event route */
ux_event_t *event_reminder_init(ux_event_t *e, ux_clock_type ctype, reminder_cb callback, void *data);
ux_event_t* event_tick_init(ux_event_t *e, int provider, int instrument, datetime_t exchange_timestamp, double price, long size);
ux_event_t* event_news_init(ux_event_t *e, int provider, int instrument, char *urgency, char *url, char *headline, char *text);
void event_news_destory(ux_event_t *e);
ux_event_t* event_fundamental_init(ux_event_t *e, int provider, int instrument);
ux_event_t* event_on_simulator_start_init(ux_event_t *e, datetime_t time1, datetime_t time2, long long count);
ux_event_t* event_execption_init(ux_event_t *e, char* source);

#endif // __UX_EVENT_H__
