#ifndef __UX_INTERNAL_H__
#define __UX_INTERNAL_H__

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <uv.h>
#include <ux.h>
#include "ux_atomic.h"
#include <assert.h>

#define UX_ASSERT(x) assert((x))
#define UX_PRINT_FOMAT_CHECK(x,y)

#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))


/* Needed everywhere. */
#include <string.h>
#include <stdlib.h>

/* Various macros. */
#ifndef UX_UNUSED
#define UX_UNUSED(x)	((void)(x))	/* to avoid warnings */
#endif

#if defined(__GNUC__)

#define UX_NORET	__attribute__((noreturn))
#define UX_ALIGN(n)	__attribute__((aligned(n)))
#define UX_INLINE	inline
#define UX_AINLINE	inline __attribute__((always_inline))
#define UX_NOINLINE	__attribute__((noinline))

#define UX_LIKELY(x)	__builtin_expect(!!(x), 1)
#define UX_UNLIKELY(x)	__builtin_expect(!!(x), 0)

#elif defined(_MSC_VER)

#define UX_NORET	__declspec(noreturn)
#define UX_ALIGN(n)	__declspec(align(n))
#define UX_INLINE	__inline
#define UX_AINLINE	__forceinline
#define UX_NOINLINE	__declspec(noinline)

#else
#error "missing defines for your compiler"
#endif

/* Optional defines. */
#ifndef UX_NORET
#define UX_NORET
#endif
#ifndef UX_NOAPI
#define UX_NOAPI	extern
#endif
#ifndef UX_LIKELY
#define UX_LIKELY(x)	(x)
#define UX_UNLIKELY(x)	(x)
#endif

/* Attributes for internal functions. */
#if defined(UX_AMALG_C)
#define UX_FUNC		static
#else
#define UX_FUNC		UX_NOAPI
#endif

/* Static assertions. */
#define UX_ASSERT_NAME2(name, line)	name ## line
#define UX_ASSERT_NAME(line)		UX_ASSERT_NAME2(lj_assert_, line)
#ifdef __COUNTER__
#define UX_STATIC_ASSERT(cond) \
  extern void UX_ASSERT_NAME(__COUNTER__)(int STATIC_ASSERTION_FAILED[(cond)?1:-1])
#else
#define UX_STATIC_ASSERT(cond) \
  extern void UX_ASSERT_NAME(__LINE__)(int STATIC_ASSERTION_FAILED[(cond)?1:-1])
#endif

/* event  module */
UX_FUNC   void ux_event_dispatch(void *loop, ux_event_t *e);
UX_FUNC ux_event_t* event_tick_init(ux_event_t *e, int provider, int instrument, datetime_t exchange_timestamp, double price, long size);

UX_FUNC ux_event_t* event_news_init(ux_event_t *e, int provider, int instrument, char *urgency, char *url, char *headline, char *text);
UX_FUNC void event_news_destory(ux_event_t *e);
UX_FUNC ux_event_t* event_news_clone(ux_event_t *e);

UX_FUNC ux_event_t* event_fundamental_init(ux_event_t *e, int provider, int instrument);
UX_FUNC ux_event_t* event_simulator_start_init(ux_event_t *e, datetime_t time1, datetime_t time2, long long count);

UX_FUNC ux_event_t* event_exception_init(ux_event_t *e, char *source);
UX_FUNC void event_exception_destory(ux_event_t *e);
UX_FUNC ux_event_t* event_exception_clone(ux_event_t *e);

/* queue module */
#include "ux_spscq.h"
#include "ux_mpscq.h"

typedef enum {
    UX_CATEGORY_MARKET = 0,
    UX_CATEGORY_EXECUTION,
    UX_CATEGORY_SERVICE,
    UX_CATEGORY_COMMAND,
    UX_CATEGORY_LAST
}ux_queue_category;

typedef struct ux_queue_s ux_queue_t;

typedef void (*ux_queue_event_handle)(ux_queue_t* q);

struct ux_queue_s {
    void *data;
    /* private */
    ux_atomic_t refcount;
    ux_spscq_t spsc;
    /* only access by consumer */
    void *loop;
    ux_queue_event_handle on_event;
    ux_queue_category category;
    /* mpsc */
    ux_mpscq_node mpsc_node;
    /* min heap node */
    void *heap_node[3];
    uint64_t start_id;
};

UX_FUNC ux_queue_t* ux_queue_init(ux_queue_t* q, unsigned int size, ux_queue_category category);

UX_FUNC void ux_queue_destory(ux_queue_t *q);

UX_FUNC void ux_queue_ref(ux_queue_t *q);

UX_FUNC void ux_queue_unref(ux_queue_t *q);

UX_EXTERN int ux_queue_push(ux_queue_t *q, void *e);

//UX_EXTERN void ux_queue_close(ux_queue_t *q);

UX_FUNC unsigned int ux_queue_size(ux_queue_t *q);

UX_FUNC unsigned int ux_queue_capacity(ux_queue_t *q);

UX_FUNC void *ux_queue_pop(ux_queue_t *q);

UX_FUNC void *ux_queue_peek(const ux_queue_t *q);

UX_FUNC int ux_queue_is_empty(ux_queue_t *q);

UX_FUNC int ux_queue_is_full(ux_queue_t *q);

typedef struct {
    void *min;
    unsigned int nelts;
}min_heap;

typedef enum {
    UX_BUS_SIMULATION,
    UX_BUS_REALTIME,
    UX_BUS_MODE_LAST
}ux_bus_mode;

typedef struct ux_loop_s ux_loop_t;
typedef struct ux_async_s ux_async_t;

typedef void (*ux_async_cb)(ux_loop_t *loop, void *data);

struct ux_async_s {
    ux_async_cb async_cb;
    void *data;
    ux_loop_t *loop;
    ux_mpscq_node mpsc_node;
};

struct ux_loop_s {
    uv_mutex_t wait_mutex;
    uv_cond_t  wait_cond;
    int stop_flag;
    ux_mpscq_t async_queue;
    /* bus field */
    ux_bus_mode mode;
    min_heap queue_heap[UX_CATEGORY_LAST];
    min_heap timer_heap[UX_CLOCK_LAST];
    uint64_t counter;
    datetime_t time[UX_CLOCK_LAST];
    ux_event_t *saved_event;
    ux_queue_t* attached[128];
    int attached_count;
    int is_simulator_stop;
};

typedef enum {
    UX_RUN_DEFAULT = 0,
    UX_RUN_ONCE,
    UX_RUN_NOWAIT
}ux_run_mode;

UX_EXTERN void ux_loop_init(ux_loop_t *loop);
UX_EXTERN void ux_loop_destory(ux_loop_t *loop);
UX_EXTERN void ux_loop_run(ux_loop_t *loop, ux_run_mode mode);
UX_EXTERN void ux_loop_stop(ux_loop_t *loop);
UX_EXTERN void ux_loop_wakeup(ux_loop_t* loop);

UX_FUNC void ux_bus_init(ux_loop_t *bus, ux_bus_mode mode);
UX_FUNC void ux_bus_destory(ux_loop_t *bus);

UX_FUNC void ux_bus_clear(ux_loop_t *bus);

UX_FUNC void ux_bus_attach(ux_loop_t* src, ux_loop_t *dst);
UX_FUNC void ux_bus_detach(ux_loop_t* src, ux_loop_t *dst);

UX_FUNC void ux_bus_add_queue(ux_loop_t *bus, void *q);
UX_FUNC void ux_bus_remove_queue(ux_loop_t *bus, void *q);

UX_FUNC void ux_bus_add_timer(ux_loop_t *bus, ux_event_reminder_t* timer);
UX_FUNC void ux_bus_remove_timer(ux_loop_t *bus, ux_event_reminder_t* timer);

UX_FUNC ux_event_t* ux_bus_next_event(ux_loop_t *bus);
UX_FUNC int64_t ux_bus_next_timeout(ux_loop_t* bus);
UX_FUNC ux_event_reminder_t* ux_bus_next_reminder(ux_loop_t* bus, ux_clock_type type);

UX_FUNC datetime_t ux_bus_get_time(ux_loop_t* bus);
UX_FUNC int ux_bus_set_time(ux_loop_t* bus, datetime_t time);

UX_FUNC datetime_t ux_bus_get_exchange_time(ux_loop_t* bus);
UX_FUNC int ux_bus_set_exchange_time(ux_loop_t* bus, datetime_t time);

//UX_FUNC void ux_async_init(ux_async_t *async, loop, ux_async_cb callback, void *data);
/* thread safe */
UX_EXTERN void ux_async_post(ux_loop_t *loop, ux_async_cb async_cb, void *data);

void ux_loop_add_queue(ux_loop_t *loop, void *q);
//void ux_loop_remove_queue(ux_loop_t *loop, void *q);


/* dispatch module */

UX_FUNC void ux_event_default_dispatch(ux_loop_t *loop, ux_event_t *e);

UX_FUNC void ux_event_reminder_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void ux_event_ask_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void ux_event_bid_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void ux_event_trade_dispatch(ux_loop_t *loop, ux_event_t *e);

#endif // __UX_INTERNAL_H__
