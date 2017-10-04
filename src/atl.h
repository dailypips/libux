/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef ATL_H
#define ATL_H

#include <stdint.h>
#include <atl/atomic/atm.h>

/* atl public api */

typedef uint64_t datetime_t;

typedef struct atl_queue_s atl_queue_t;
typedef struct atl_loop_s  atl_loop_t;
typedef struct atl_async_s atl_async_t;
typedef  struct atl_event_s atl_event_t;

typedef enum {
  ATL_EVENT_FLAG_NONE         = 1 << 0,
  ATL_EVENT_FLAG_MARKET       = 1 << 1,
  ATL_EVENT_FLAG_TICK         = 1 << 2,
  ATL_EVENT_FLAG_EXECUTION    = 1 << 3,
  ATL_EVENT_FLAG_SERVICE      = 1 << 4,
  ATL_EVENT_FLAG_REMINDER     = 1 << 5
} atl_event_flag;


#define EVENTDEF(_) \
    /* UpperName | lowcaseName | init |  destory | clone | flag */ \
    _(REMINDER, reminder, event_reminder_init, NULL, NULL, ATL_EVENT_FLAG_REMINDER) \
    _(ASK,      ask,      event_tick_init,     NULL, NULL, ATL_EVENT_FLAG_MARKET | ATL_EVENT_FLAG_TICK) \
    _(BID,      bid,      event_tick_init,     NULL, NULL, ATL_EVENT_FLAG_MARKET | ATL_EVENT_FLAG_TICK) \
    _(TRADE,    trade,    event_tick_init,     NULL, NULL, ATL_EVENT_FLAG_MARKET | ATL_EVENT_FLAG_TICK) \
    _(BAR,      bar,      event_tick_init,     NULL, NULL, ATL_EVENT_FLAG_MARKET) \
    _(NEWS,     news,     event_news_init,     event_news_destory, event_news_clone, ATL_EVENT_FLAG_MARKET) \
    _(FUNDAMENTAL, fundamental, event_fundamental_init, NULL, NULL, ATL_EVENT_FLAG_MARKET) \
    /* internal event */ \
    _(QUEUE_OPENED, queue_opend, event_queue_init, NULL, NULL, ATL_EVENT_FLAG_NONE) \
    _(QUEUE_CLOSED, queue_opend, event_queue_init, NULL, NULL, ATL_EVENT_FLAG_NONE) \

/* Bytecode opcode numbers. */
typedef enum {
        ATL_EVENT_UNKNOW = 0,
#define EVENTENUM(name, lname, init, destory, clone, flag)	ATL_EVENT_##name,
EVENTDEF(EVENTENUM)
#undef EVENTENUM
    /* custom event */
    ATL_EVENT_CUSTOM_START = 512,
      /* last */
    ATL_EVENT_LAST = 1023
} atl_event_type;

#define ATL_EVENT_PUBLIC_FIELDS \
    atl_atm refcount;        \
    atl_event_type type;        \
    atl_event_flag flag;        \
    datetime_t timestamp;       \

struct atl_event_s {
  ATL_EVENT_PUBLIC_FIELDS
};

atl_event_t* atl_event_new(atl_event_type type, ...);
atl_event_t* atl_event_ref(atl_event_t *e);
void atl_event_unref(atl_event_t *e);
atl_event_t* atl_event_clone(atl_event_t *e);

typedef enum {
    ATL_CATEGORY_MARKET = 0,
    ATL_CATEGORY_EXECUTION,
    ATL_CATEGORY_SERVICE,
    ATL_CATEGORY_COMMAND,
    ATL_CATEGORY_LAST
}atl_queue_category;

typedef enum {
    ATL_CLOCK_LOCAL,
    ATL_CLOCK_EXCHANGE,
    ATL_CLOCK_LAST
}atl_clock_type;

/*
 * size must be > 2
 * push QUEUE_CLOSE to free atl_queue
 */
atl_queue_t* atl_queue_new(atl_loop_t *loop, unsigned int size, atl_queue_category category);

/*
 * return 0 means successful
 * -1 means queue is full
 */
int atl_queue_push(atl_queue_t *q, void *e);


typedef enum {
    ATL_RUN_DEFAULT = 0,
    ATL_RUN_ONCE,
    ATL_RUN_NOWAIT
}atl_run_mode;

/* loop */
atl_loop_t* atl_loop_new(void);

void atl_run(atl_loop_t *loop, atl_run_mode mode);
void atl_stop(atl_loop_t *loop);

typedef void (*atl_async_cb)(atl_loop_t *loop, void *data);

void atl_async_post(atl_loop_t *loop, atl_async_cb async_cb, void *data);

datetime_t atl_time(atl_loop_t *loop);
void atl_set_time(atl_loop_t *loop, datetime_t time);
datetime_t atl_exchange_time(atl_loop_t *loop);
void atl_set_exchange_time(atl_loop_t *loop, datetime_t time);

void atl_attach(atl_loop_t *loop);
void atl_detach(atl_loop_t *loop);

/* time api */
datetime_t atl_utc_now(void);
datetime_t atl_now(void);

/* timer api */
/* timer的实现不同于其他事件的实现方式,其他事件不用包含heap_node
 * 原因在于事件本身不需要优先级管理，所在的队列需要heap_node来实现优先级
 * timer本身要实现优先级管理所以要包含heap_node
 * 另外一种考虑的方式就是在push reminder的时候转化为一个timer，
 * timer结构对用户不透明
 */
typedef void (*atl_timer_cb)(atl_loop_t *loop, datetime_t time);
void atl_add_timer(atl_loop_t *loop, atl_timer_cb cb, datetime_t start, datetime_t timeout, datetime_t repeat);

#endif // ATL_H
