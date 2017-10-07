/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

/** @file 
 * Libux C API header file.
 * The elements declared here constitute the Libux C API.
 */

#ifndef __UX_H__
#define __UX_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  /* Windows - set up dll import/export decorators. */
# if defined(BUILDING_UX_SHARED)
    /* Building shared library. */
#   define UX_EXTERN __declspec(dllexport)
# elif defined(USING_UX_SHARED)
    /* Using shared library. */
#   define UX_EXTERN __declspec(dllimport)
# else
    /* Building static library. */
#   define UX_EXTERN /* nothing */
# define UX_HIDDEN
# endif
#elif __GNUC__ >= 4
# define UX_EXTERN __attribute__((visibility("default")))
# define UX_HIDDEN __attribute__ ((visibility ("hidden")))
#else
# define UX_EXTERN /* nothing */
#endif


#include <stddef.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
# include "stdint-msvc2008.h"
#else
# include <stdint.h>
#endif

#include <sys/time.h> // timeval

/* memory module */
typedef void* (*ux_malloc_func)(size_t size);
typedef void* (*ux_realloc_func)(void* ptr, size_t size);
typedef void* (*ux_calloc_func)(size_t count, size_t size);
typedef void (*ux_free_func)(void* ptr);

UX_EXTERN int ux_replace_allocator(ux_malloc_func malloc_func,
                                   ux_realloc_func realloc_func,
                                   ux_calloc_func calloc_func,
                                   ux_free_func free_func);

UX_EXTERN void* ux_malloc(size_t size);
UX_EXTERN void* ux_zalloc(size_t size);
UX_EXTERN void  ux_free(void *ptr);
UX_EXTERN void* ux_realloc(void *ptr, size_t size);
UX_EXTERN void* ux_malloc_aligned(size_t size, size_t alignment);
UX_EXTERN char *ux_strdup(const char *src);

/* datetime module */
/* datetime_t type represents a date and time.
   that stores the date and time as the number of microsecond intervals since
   12:00 AM January 1, year 1 A.D (as Day 0). in the proleptic Gregorian Calendar.
*/

typedef uint64_t datetime_t;
typedef int64_t  timespan_t;

#define EPOCH_DATE_TIME UINT64_C(62135596800000000)  /* 1970-01-01T00:00:00:00 */
#define MIN_DATE_TIME UINT64_C(0)                    /* 0001-01-01T00:00:00:00 */
#define MAX_DATE_TIME UINT64_C(315537897599999999)   /* 9999-12-31T23:59:59.999999*/

UX_EXTERN datetime_t datetime_from_ymd(uint16_t year, uint16_t month, uint16_t day);
UX_EXTERN datetime_t datetime_from_hmsu(uint16_t hour, uint16_t minute, uint16_t second, uint32_t usec);
UX_EXTERN datetime_t datetime_from_timeval(struct timeval *t);
/* result = 0 means ok, other means error */
UX_EXTERN int datetime_from_iso8601(const char *str, size_t len, datetime_t *dt);
UX_EXTERN void datetime_decode(datetime_t dt, uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t *minute, uint16_t *second, uint32_t *usec);
UX_EXTERN int datetime_to_tm(const datetime_t dt, struct tm *tmp);
UX_EXTERN size_t datetime_format(char *dst, size_t len, datetime_t dt, long offset /* see timeout_offset */);
/* return timezone offset from utc, in minutes [-1439,1439] */
UX_EXTERN long get_timezone_offset(void);
UX_EXTERN void ux_timezone_update(void);
UX_EXTERN datetime_t datetime_now(void);

/* atomic module */
typedef intptr_t ux_atomic_t;

/* event module */
#define EVENTDEF(_)                                                                      \
    /* UppercaseName        LowcaseName         destory     clone        dispatch*/      \
    _(REMINDER,             reminder,           default,    default,     default)        \
    _(ASK,                  ask,                default,    default,     default)        \
    _(BID,                  bid,                default,    default,     default)        \
    _(TRADE,                trade,              default,    default,     default)        \
    _(L2SNAPSHOT,           l2snapshot,         l2snapshot, l2snapshot,  default)        \
    _(L2UPDATE,             l2update,           l2update,   l2update,    default)        \
    /*_(BAR,                bar,                default,    default,     bar)*/          \
    _(NEWS,                 news,               news,       news,        default)        \
    _(FUNDAMENTAL,          fundamental,        default,    default,     default)        \
    /* internal event */                                                                 \
    _(QUEUE_OPENED,         queue_opened,       default,    default,     default)        \
    _(QUEUE_CLOSED,         queue_closed,       default,    default,     default)        \
    _(SIMULATOR_START,      simulator_start,    default,    default,     default)        \
    _(SIMULATOR_STOP,       simulator_stop,     default,    default,     default)        \
    _(SIMULATOR_PROGRESS,   simulator_progress, default,    default,     default)        \
    _(EXCEPTION,            exception,          exception,  exception,   default)


typedef enum {
#define EVENTENUM(name, lname, destory, clone, processor)	UX_EVENT_##name,
EVENTDEF(EVENTENUM)
#undef EVENTENUM
    UX_EVENT_LAST
} ux_event_type;

#ifdef NDEBUG
#define UX_EVENT_PUBLIC_FIELDS \
    ux_atomic_t refcount;        \
    ux_event_type type;        \
    datetime_t timestamp;
#else
#define UX_EVENT_PUBLIC_FIELDS \
    ux_atomic_t refcount;        \
    ux_event_type type;        \
    datetime_t timestamp; \
    void* dummy;
#endif

typedef struct ux_event_s {
  UX_EVENT_PUBLIC_FIELDS
}ux_event_t;

UX_EXTERN size_t ux_event_size(ux_event_type type);
UX_EXTERN ux_event_t* ux_event_malloc(ux_event_type type);
UX_EXTERN ux_event_t* ux_event_ref(ux_event_t *e);
UX_EXTERN void ux_event_unref(ux_event_t *e);
UX_EXTERN ux_event_t* ux_event_clone(ux_event_t *e);

/* events */
/* reminder event */
typedef enum {
    UX_CLOCK_LOCAL  = 0,
    UX_CLOCK_EXCHANGE,
    UX_CLOCK_LAST
}ux_clock_type;

typedef void (*ux_reminder_cb)(datetime_t time, void *data);

typedef struct ux_event_reminder_s {
    UX_EVENT_PUBLIC_FIELDS
    ux_clock_type clock_type;
    ux_reminder_cb callback;
    void *user_data;
    datetime_t start;
    datetime_t timeout;
    datetime_t repeat;
    /*private*/
    void *loop;
    void* heap_node[3];
    uint64_t start_id;
}ux_event_reminder_t;

/* market data event */
#define UX_EVENT_COMMON_FIELDS \
    int provider;           \
    int instrument;

#define UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME \
    UX_EVENT_COMMON_FIELDS \
    datetime_t exchange_timestamp;

typedef struct tick_event_s {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    double price;
    long size;
} ux_event_tick_t;

typedef ux_event_tick_t ux_event_ask_t;
typedef ux_event_tick_t ux_event_bid_t;
typedef ux_event_tick_t ux_event_trade_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS
    char *urgency;
    char *url;
    char *headline;
    char *text;
} ux_event_news_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS
} ux_event_fundamental_t;

typedef struct {
    double price;
    long size;
}ux_tick_info_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    size_t num_bids;
    size_t num_asks;
    ux_tick_info_t* ticks;
}ux_event_l2snapshot_t;

typedef enum {
    UX_L2_ACTION_NEW,
    UX_L2_ACTION_CHANGE,
    UX_L2_ACTION_DELETE,
    UX_L2_ACTION_RESET
}ux_level2_action;

typedef enum {
    UX_SIDE_BID,
    UX_SIDE_ASK,
}ux_level2_side;

typedef struct {
    ux_level2_side side;
    ux_level2_action action;
    int position;
}ux_level2_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    size_t num_entries;
    ux_level2_t *entries;
}ux_event_l2update_t;

/* system */
typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    void *q;
}ux_event_queue_t;

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
}ux_event_simulator_start_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_simulator_stop_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
}ux_event_simulator_progress_t;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    char *source;
}ux_event_exception_t;

typedef ux_event_queue_t ux_event_queue_opened_t;
typedef ux_event_queue_t ux_event_queue_closed_t;

/* queue module */
typedef enum {
    UX_CATEGORY_MARKET = 0,
    UX_CATEGORY_EXECUTION,
    UX_CATEGORY_SERVICE,
    UX_CATEGORY_COMMAND,
    UX_CATEGORY_LAST
}ux_queue_category;

typedef struct ux_queue_s ux_queue_t;

UX_EXTERN ux_queue_t* ux_queue_new(unsigned int size, ux_queue_category category);
UX_EXTERN int ux_queue_push(ux_queue_t *q, void *e);

/* loop module */
typedef struct ux_loop_s ux_loop_t;
typedef struct ux_async_s ux_async_t;
typedef void (*ux_async_cb)(ux_loop_t *loop, void *data);

struct ux_async_s {
    ux_async_cb async_cb;
    void *data;
    /* private */
    ux_atomic_t next;
};

typedef enum {
    UX_BUS_SIMULATION,
    UX_BUS_REALTIME,
    UX_BUS_MODE_LAST
}ux_bus_mode;

UX_EXTERN ux_loop_t* ux_loop_new(void);
UX_EXTERN void ux_loop_free(ux_loop_t *loop);
UX_EXTERN void ux_stop(ux_loop_t *loop);
UX_EXTERN void ux_wakeup(ux_loop_t* loop);

#ifdef __cplusplus
}
#endif

#endif /*__UX_H__*/
