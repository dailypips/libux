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
#include <time.h>   // struct tm

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

typedef uint64_t ux_time_t;
typedef int64_t  ux_timespan_t;

#define EPOCH_DATE_TIME UINT64_C(62135596800000000)  /* 1970-01-01T00:00:00:00 */
#define MIN_DATE_TIME UINT64_C(0)                    /* 0001-01-01T00:00:00:00 */
#define MAX_DATE_TIME UINT64_C(315537897599999999)   /* 9999-12-31T23:59:59.999999*/

#define TICKS_PER_SECOND UINT64_C(1000000)
#define TICKS_PER_DAY UINT64_C(86400000000)

UX_EXTERN ux_time_t ux_time_from_ymd(uint16_t year, uint16_t month, uint16_t day);
UX_EXTERN ux_time_t ux_time_from_hmsu(uint16_t hour, uint16_t minute, uint16_t second, uint32_t usec);
UX_EXTERN ux_time_t ux_time_from_timeval(struct timeval *t);
/* result = 0 means ok, other means error */
UX_EXTERN int ux_time_from_iso8601(const char *str, size_t len, ux_time_t *dt);
UX_EXTERN void ux_time_decode(ux_time_t dt, uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t *minute, uint16_t *second, uint32_t *usec);
UX_EXTERN int ux_time_to_tm(const ux_time_t dt, struct tm *tmp);
UX_EXTERN size_t ux_time_format(char *dst, size_t len, ux_time_t dt, long offset /* see timeout_offset */);
/* return timezone offset from utc, in minutes [-1439,1439] */
UX_EXTERN long ux_get_timezone_offset(void);
UX_EXTERN void ux_timezone_update(void);
UX_EXTERN ux_time_t ux_time_now(void);

/* atomic module */
typedef intptr_t ux_atomic_t;

/* ids */
typedef uint16_t ux_iid_t;   /* for instrument id, range[1,65534] */
#define UX_INSTRUMENT_ID_MIN 1
#define UX_INSTRUMENT_ID_MAX 65534

typedef uint8_t ux_pid_t;   /* for provider id, range[1,254] */
//TODO: 可以取 -1
#define UX_UNKNOWN_PROVDER -1
#define UX_PROVIDER_ID_MIN 1
#define UX_PROVIDRE_ID_MAX 254

/* currency module */
typedef uint16_t ux_currency_t;

/* iso 4217 */
#define CURRENCY_DEF(_) \
  _(NONE, 0)            \
  _(AED, 784)           \
  _(AFN, 971)           \
  _(ALL, 8)             \
  _(AMD, 51)            \
  _(ANG, 532)           \
  _(AOA, 973)           \
  _(ARS, 32)            \
  _(AUD, 36)            \
  _(AWG, 533)           \
  _(AZN, 944)           \
  _(BAM, 977)           \
  _(BBD, 52)            \
  _(BDT, 50)            \
  _(BGN, 975)           \
  _(BHD, 48)            \
  _(BIF, 108)           \
  _(BMD, 60)            \
  _(BND, 96)            \
  _(BOB, 68)            \
  _(BOV, 984)           \
  _(BRL, 986)           \
  _(BSD, 44)            \
  _(BTN, 64)            \
  _(BWP, 72)            \
  _(BYR, 974)           \
  _(BZD, 84)            \
  _(CAD, 124)           \
  _(CDF, 976)           \
  _(CHE, 947)           \
  _(CHF, 756)           \
  _(CHW, 948)           \
  _(CLF, 990)           \
  _(CLP, 152)           \
  _(CNH, 156)           \
  _(CNY, 156)           \
  _(COP, 170)           \
  _(COU, 970)           \
  _(CRC, 188)           \
  _(CUC, 931)           \
  _(CUP, 192)           \
  _(CVE, 132)           \
  _(CZK, 203)           \
  _(DJF, 262)           \
  _(DKK, 208)           \
  _(DOP, 214)           \
  _(DZD, 12)            \
  _(EGP, 818)           \
  _(ERN, 232)           \
  _(ETB, 230)           \
  _(EUR, 978)           \
  _(FJD, 242)           \
  _(FKP, 238)           \
  _(GBP, 826)           \
  _(GEL, 981)           \
  _(GHS, 936)           \
  _(GIP, 292)           \
  _(GMD, 270)           \
  _(GNF, 324)           \
  _(GTQ, 320)           \
  _(GYD, 328)           \
  _(HKD, 344)           \
  _(HNL, 340)           \
  _(HRK, 191)           \
  _(HTG, 332)           \
  _(HUF, 348)           \
  _(IDR, 360)           \
  _(ILS, 376)           \
  _(INR, 356)           \
  _(IQD, 368)           \
  _(IRR, 364)           \
  _(ISK, 352)           \
  _(JMD, 388)           \
  _(JOD, 400)           \
  _(JPY, 392)           \
  _(KES, 404)           \
  _(KGS, 417)           \
  _(KHR, 116)           \
  _(KMF, 174)           \
  _(KPW, 408)           \
  _(KRW, 410)           \
  _(KWD, 414)           \
  _(KYD, 136)           \
  _(KZT, 398)           \
  _(LAK, 418)           \
  _(LBP, 422)           \
  _(LKR, 144)           \
  _(LRD, 430)           \
  _(LSL, 426)           \
  _(LTL, 440)           \
  _(LYD, 434)           \
  _(MAD, 504)           \
  _(MDL, 498)           \
  _(MGA, 969)           \
  _(MKD, 807)           \
  _(MMK, 104)           \
  _(MNT, 496)           \
  _(MOP, 446)           \
  _(MRO, 478)           \
  _(MUR, 480)           \
  _(MVR, 462)           \
  _(MWK, 454)           \
  _(MXN, 484)           \
  _(MXV, 979)           \
  _(MYR, 458)           \
  _(MZN, 943)           \
  _(NAD, 516)           \
  _(NGN, 566)           \
  _(NIO, 558)           \
  _(NOK, 578)           \
  _(NPR, 524)           \
  _(NZD, 554)           \
  _(OMR, 512)           \
  _(PAB, 590)           \
  _(PEN, 604)           \
  _(PGK, 598)           \
  _(PHP, 608)           \
  _(PKR, 586)           \
  _(PLN, 985)           \
  _(PYG, 600)           \
  _(QAR, 634)           \
  _(RON, 946)           \
  _(RSD, 941)           \
  _(RUB, 643)           \
  _(RWF, 646)           \
  _(SAR, 682)           \
  _(SBD, 90)            \
  _(SCR, 690)           \
  _(SDG, 938)           \
  _(SEK, 752)           \
  _(SGD, 702)           \
  _(SHP, 654)           \
  _(SLL, 694)           \
  _(SOS, 706)           \
  _(SRD, 968)           \
  _(SSP, 728)           \
  _(STD, 678)           \
  _(SYP, 760)           \
  _(SZL, 748)           \
  _(THB, 764)           \
  _(TJS, 972)           \
  _(TMT, 934)           \
  _(TND, 788)           \
  _(TOP, 776)           \
  _(TRY, 949)           \
  _(TTD, 780)           \
  _(TWD, 901)           \
  _(TZS, 834)           \
  _(UAH, 980)           \
  _(UGX, 800)           \
  _(USD, 840)           \
  _(USN, 997)           \
  _(USS, 998)           \
  _(UYI, 940)           \
  _(UYU, 858)           \
  _(UZS, 860)           \
  _(VEF, 937)           \
  _(VND, 704)           \
  _(VUV, 548)           \
  _(WST, 882)           \
  _(XAF, 950)           \
  _(XAG, 961)           \
  _(XAU, 959)           \
  _(XBA, 955)           \
  _(XBB, 956)           \
  _(XBC, 957)           \
  _(XBD, 958)           \
  _(XCD, 951)           \
  _(XDR, 960)           \
  _(XOF, 952)           \
  _(XPD, 964)           \
  _(XPF, 953)           \
  _(XPT, 962)           \
  _(XSU, 994)           \
  _(XTS, 963)           \
  _(XUA, 965)           \
  _(XXX, 999)           \
  _(YER, 886)           \
  _(ZAR, 710)           \
  _(ZMW, 967)           \
  _(ZWD, 932)

#define ISO4217_CURRENCY_LAST 1024

ux_currency_t ux_currency_from_name(const char *name);
const char* ux_currency_to_name(ux_currency_t currency);

/* event module */
#define EVENTDEF(_)                                                                                   \
    /* UppercaseName        LowcaseName         destory            clone              dispatch*/      \
     /* market events */                                                                              \
    _(REMINDER,             reminder,           default,           default,           reminder)       \
    _(ASK,                  ask,                default,           default,           ask)            \
    _(BID,                  bid,                default,           default,           bid)            \
    _(TRADE,                trade,              default,           default,           trade)          \
    _(L2SNAPSHOT,           l2snapshot,         l2snapshot,        l2snapshot,        default)        \
    _(L2UPDATE,             l2update,           l2update,          l2update,          default)        \
    _(BAR,                  bar,                default,           default,           default)        \
    _(NEWS,                 news,               news,              news,              default)        \
    _(FUNDAMENTAL,          fundamental,        default,           default,           default)        \
    /* execution events */                                                                            \
    _(ACCOUNT_REPORT,       account_report,     account_report,    account_report,    default)        \
    _(EXECUTION_REPORT,     execution_report,   execution_report,  execution_report,  default)        \
    _(EXECUTION_COMMAND,    execution_command,  execution_command, execution_command, default)        \
    /* internal events */                                                                             \
    _(QUEUE_OPENED,         queue_opened,       default,           default,           default)        \
    _(QUEUE_CLOSED,         queue_closed,       default,           default,           default)        \
    _(SIMULATOR_START,      simulator_start,    default,           default,           default)        \
    _(SIMULATOR_STOP,       simulator_stop,     default,           default,           default)        \
    _(SIMULATOR_PROGRESS,   simulator_progress, default,           default,           default)        \
    _(EXCEPTION,            exception,          exception,         exception,         default)


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
    ux_time_t timestamp;
#else
#define UX_EVENT_PUBLIC_FIELDS \
    ux_atomic_t refcount;        \
    ux_event_type type;        \
    ux_time_t timestamp; \
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

typedef struct ux_event_reminder_s ux_event_reminder_t;

typedef void (*ux_reminder_cb)(ux_event_reminder_t *timer);

struct ux_event_reminder_s {
    UX_EVENT_PUBLIC_FIELDS
    ux_clock_type clock_type;
    ux_reminder_cb callback;
    void *user_data;
    ux_time_t timeout;
    uint64_t repeat;
    /*private*/
    void *loop;
    void* heap_node[3];
    uint64_t start_id;
};

/* market data event */
#define UX_EVENT_COMMON_FIELDS \
    int provider;           \
    int instrument;

#define UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME \
    UX_EVENT_COMMON_FIELDS \
    ux_time_t exchange_timestamp;

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
    UX_L2SIDE_BID,
    UX_L2SIDE_ASK,
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

typedef enum {
    UX_BAR_TYPE_TIME,
    UX_BAR_TYPE_TICK,
    UX_BAR_TYPE_VOLUME,
    UX_BAR_TYPE_RANGE,
    UX_BAR_TYPE_SESSION,
    UX_BAR_TYPE_RENKO,
    UX_BAR_TYPE_CUSTOM
}ux_bar_type;

typedef enum {
    UX_BAR_STATUS_INCOMPLETE,
    UX_BAR_STATUS_COMPLETE,
    UX_BAR_STATUS_OPEN,
    UX_BAR_STATUS_HIGH,
    UX_BAR_STATUS_LOW,
    UX_BAR_STATUS_CLOSE
}ux_bar_status;

typedef enum {
    UX_BAR_INPUT_TRADE,
    UX_BAR_INPUT_BID,
    UX_BAR_INPUT_ASK,
    UX_BAR_INPUT_MIDDLE,
    UX_BAR_INPUT_TICK,
    UX_BAR_INPUT_BIDASK
}ux_bar_input;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_COMMON_FIELDS_WITH_EXCHANGE_TIME
    ux_bar_type bar_type;
    long size;
    ux_time_t open_time;
    double high;
    double low;
    double open;
    double close;
    long volume;
    long open_int;
    long tick_count;
    ux_bar_status status;
}ux_event_bar_t;

static double ux_bar_get_range(ux_event_bar_t *bar)
{
    return bar->high - bar->low;
}

static double ux_bar_get_median(ux_event_bar_t *bar)
{
    return (bar->high + bar->low) / 2.0;
}

static double ux_bar_get_typical(ux_event_bar_t *bar)
{
    return (bar->high + bar->low + bar->close) / 3.0;
}

static double ux_bar_get_weighted(ux_event_bar_t *bar)
{
    return (bar->high + bar->low + bar->close * 2.0) / 4.0;
}

static double ux_bar_get_average(ux_event_bar_t *bar)
{
    return (bar->high + bar->low + bar->open+ bar->close) / 4.0;
}

/* execution */
#define UX_EVENT_EXECUTION_PUBLIC_FIELDS \
    int provider;   \
    int instrument; \
    int id; \
    int order_id; \
    char *client_order_id; \
    char *provider_order_id; \
    int client_id; \
    int is_loadded; \
    ux_currency_t currency; \

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_EXECUTION_PUBLIC_FIELDS
    int portfolio;
    double amount;
    char *text;
}ux_event_account_report_t;


typedef enum {
    UX_EXEC_NEW,
    UX_EXEC_STOPPED,
    UX_EXEC_REJECTED,
    UX_EXEC_EXPIRED,
    UX_EXEC_TRADE,
    UX_EXEC_PENDING_CANCEL,
    UX_EXEC_CANCELLED,
    UX_EXEC_CANCEL_REJECT,
    UX_EXEC_PENDING_REPLACE,
    UX_EXEC_REPLACE,
    UX_EXEC_REPLACE_REJECT,
    UX_EXEC_TRADE_CORRECT,
    UX_EXEC_TRADE_CANCEL,
    UX_EXEC_ORDER_STATUS,
    UX_EXEC_PENDING_NEW,
    UX_EXEC_CLEARING_HOLD
}ux_exec_type;

typedef enum {
    UX_ORDER_TYPE_MARKET,
    UX_ORDER_TYPE_STOP,
    UX_ORDER_TYPE_LIMIT,
    UX_ORDER_TYPE_STOP_LIMIT,
    UX_ORDER_TYPE_MARKET_CLOSE,
    UX_ORDER_TYPE_PEGGED,
    UX_ORDER_TYPE_TRAILING_STOP,
    UX_ORDER_TYPE_TRAILING_STOP_LIMIT
}ux_order_type;

typedef enum {
    UX_ORDER_STATUS_NOT_SENT,
    UX_ORDER_STATUS_PENDING_NEW,
    UX_ORDER_STATUS_NEW,
    UX_ORDER_STATUS_REJECTED,
    UX_ORDER_STATUS_PARTIALLY_FILLED,
    UX_ORDER_STATUS_FILLED,
    UX_ORDER_STATUS_PENDING_CANCEL,
    UX_ORDER_STATUS_CANCELLED,
    UX_ORDER_STATUS_EXPIRED,
    UX_ORDER_STATUS_PENDING_REPLACE,
    UX_ORDER_STATUS_REPLACED
}ux_order_status;

typedef enum {
    UX_TIF_ATC,
    UX_TIF_DAY,
    UX_TIF_GTC,
    UX_TIF_IOC,
    UX_TIF_OPG,
    UX_TIF_OC,
    UX_TIF_FOK,
    UX_TIF_GTX,
    UX_TIF_GTD,
    UX_TIF_GFS,
    UX_TIF_AUC
}ux_tif;

typedef enum {
    UX_SIDE_BUY,
    UX_SIDE_SELL
}ux_order_side;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_EXECUTION_PUBLIC_FIELDS
    int route_id;
    int algo;
    int portfolio;
    int strategy_id;
    ux_time_t transact_time;
    double min_qty;
    double peg_difference;
    char* exec_inst;
    char* oca;
    char* account;
    char* clientID;
    ux_exec_type exec_type;
    ux_order_type order_type;
    ux_order_side order_side;
    ux_tif tif;
    ux_time_t expire_time;
    ux_order_status order_status;
    double last_price;
    double avg_price;
    double order_qty;
    double cum_qty;
    double last_qty;
    double leaves_qty;
    double price;
    double stop_price;
    double commission;
    char* text;
    char* exec_id;
}ux_event_execution_report_t;


typedef enum {
    UX_EXECUTION_COMMAND_SEND,
    UX_EXECUTION_COMMAND_CANCEL,
    UX_EXECUTION_COMMAND_REPLACE
}ux_execution_command_type;

typedef struct {
    UX_EVENT_PUBLIC_FIELDS
    UX_EVENT_EXECUTION_PUBLIC_FIELDS
    int route_id;
    ux_execution_command_type execution_command_type;
    char* symbol;
    int algo;
    int portfolio;
    int strategy_id;
    ux_time_t transact_time;
    ux_order_side order_side;
    ux_order_type order_type;
    ux_tif tif;
    double price;
    double stop_price;
    double qty;
    double min_qty;
    double peg_difference;
    char* exec_inst;
    char* oca;
    char* text;
    char* account;
    char* clientID;
}ux_event_execution_command_t;

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
    ux_time_t time1;
    ux_time_t time2;
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

typedef enum {
    UX_RUN_DEFAULT = 0,
    UX_RUN_ONCE,
    UX_RUN_NOWAIT
}ux_run_mode;

UX_EXTERN ux_loop_t* ux_loop_new(void);
UX_EXTERN void ux_loop_free(ux_loop_t *loop);
UX_EXTERN void ux_run(ux_loop_t*loop, ux_run_mode mode);
UX_EXTERN void ux_stop(ux_loop_t *loop);
UX_EXTERN void ux_wakeup(ux_loop_t* loop); 

/* instrument module */
typedef enum {
    UX_INSTRUMENT_TYPE_STOCK,
    UX_INSTRUMENT_TYPE_FUTURE,
    UX_INSTRUMENT_TYPE_OPTION,
    UX_INSTRUMENT_TYPE_FUTUREOPTION,
    UX_INSTRUMENT_TYPE_BOND,
    UX_INSTRUMENT_TYPE_FX,
    UX_INSTRUMENT_TYPE_INDEX,
    UX_INSTRUMENT_TYPE_ETF,
    UX_INSTRUMENT_TYPE_MULTILEG,
    UX_INSTRUMENT_TYPE_SYNTHETIC
}ux_instrument_type;

typedef enum {
    UX_PUT,
    UX_CALL
}ux_putcall_type;

typedef struct ux_instrument_s {
    ux_atomic_t refcount;
    int id;
    ux_instrument_type type;
    char* symbol;
    char* description;
    char* exchange;
    ux_currency_t currency;
    ux_currency_t ccy1;
    ux_currency_t ccy2;
    double tickSize;
    ux_time_t maturity;
    double factor;
    double strike;
    ux_putcall_type putcall;
    double margin;
    char* price_format;
    char* formula;
    //AltIdList altId = new AltIdList();
    void* legs[2];
    ux_event_trade_t trade;
    ux_event_ask_t ask;
    ux_event_bid_t bid;
    ux_event_bar_t bar;
    struct ux_instrument_s *parent;
    int is_deleted    :1;
    int is_persistent :1;
    /*
  IDataProvider dataProvider;
  IExecutionProvider executionProvider;
  */
}ux_instrument_t;

UX_EXTERN void ux_instrument_init(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_destory(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_ref(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_unref(ux_instrument_t *instrument);

/* order module */
typedef struct ux_order_s {
    ux_atomic_t refcount;
    int id;
    int clId;
    int algoId;
    int clientId;
    char* providerOrderId;
    char* clOrderId;
    uint8_t providerId;
    uint8_t routeId;
    int instrumentId;
    int portfolioId;
    int strategyId;
    ux_currency_t currencyId;
    ux_time_t transactTime;
    ux_order_status orderStatus;
    ux_order_side orderSide;
    ux_order_type orderType;
    ux_tif tif;
    ux_time_t expireTime;
    double price;
    double stopPx;
    double avgPx;
    double qty;
    double cumQty;
    double leavesQty;
    double minQty;
    double pegDifference;
    char* execInst;
    char* oca;
    char* text;
    char* account;
    char* clientID;
    double queueSize;
    int isOutPrice;
    int isQueueCalculated;
    void* commands[2]; // list
    void* reports[2]; // list
    void* messages[2]; // list

    /*
    Instrument instrument;
    IExecutionProvider executionProvider;
    Portfolio portfolio;
    */
} ux_order_t;

UX_EXTERN void ux_order_init(ux_order_t *order);
UX_EXTERN void ux_order_destory(ux_order_t *order);
UX_EXTERN void ux_order_ref(ux_order_t *order);
UX_EXTERN void ux_order_unref(ux_order_t *order);

#ifdef __cplusplus
}
#endif

#endif /*__UX_H__*/
