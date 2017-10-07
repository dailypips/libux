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
#include "ux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* event  module */

/* event class info */
typedef void (*event_destory)(ux_event_t* e);
typedef ux_event_t* (*event_clone)(ux_event_t* e);
typedef void (*event_dispatch)(ux_loop_t *loop, ux_event_t* e);

typedef struct _ux_event_class_info_t {
    size_t size;
    event_destory destory;
    event_clone clone;
    event_dispatch dispatch;
} ux_event_class_info_t;

extern ux_event_class_info_t g_eventclassinfo[UX_EVENT_LAST];

/* queue module */
#include "ux_spscq.h"
#include "ux_mpscq.h"

struct ux_queue_s {
    void *data;
    /* private */
    ux_atomic_t refcount;
    spscq_t spsc;
    /* only access by consumer */
    void *loop;
    ux_queue_category category;
    /* mpsc */
    mpscq_node mpsc_node;
    /* min heap node */
    void *heap_node[3];
    uint64_t start_id;
};

UX_FUNC ux_queue_t* ux_queue_init(ux_queue_t* q, unsigned int size, ux_queue_category category);
UX_FUNC void ux_queue_destory(ux_queue_t *q);

UX_FUNC void ux_queue_ref(ux_queue_t *q);
UX_FUNC void ux_queue_unref(ux_queue_t *q);

UX_FUNC unsigned int ux_queue_size(ux_queue_t *q);
UX_FUNC unsigned int ux_queue_capacity(ux_queue_t *q);

UX_FUNC void *ux_queue_peek(const ux_queue_t *q);
UX_FUNC void *ux_queue_pop(ux_queue_t *q);

UX_FUNC int ux_queue_is_empty(ux_queue_t *q);
UX_FUNC int ux_queue_is_full(ux_queue_t *q);

typedef struct {
    void *min;
    unsigned int nelts;
}min_heap;

struct ux_loop_s {
    uv_mutex_t wait_mutex;
    uv_cond_t  wait_cond;
    int stop_flag;
    mpscq_t async_queue;
    /* bus field */
    ux_bus_mode mode;
    mpscq_t pending_queue;
    min_heap queue_heap[UX_CATEGORY_LAST];
    min_heap timer_heap[UX_CLOCK_LAST];
    uint64_t counter;
    ux_time_t time[UX_CLOCK_LAST];
    ux_event_t *saved_event;
    ux_queue_t* attached[128];
    int attached_count;
    int is_simulator_stop;
};

UX_FUNC void ux_loop_init(ux_loop_t *loop);
UX_FUNC void ux_loop_destory(ux_loop_t *loop);
UX_FUNC void ux_loop_clear(ux_loop_t *loop);

UX_FUNC void bus_attach(ux_loop_t* src, ux_loop_t *dst);
UX_FUNC void bus_detach(ux_loop_t* src, ux_loop_t *dst);

UX_FUNC void bus_add_queue(ux_loop_t *loop, ux_queue_t *q);
UX_FUNC void bus_remove_queue(ux_loop_t *loop, ux_queue_t *q);

UX_FUNC void bus_add_timer(ux_loop_t *loop, ux_event_reminder_t* timer);
UX_FUNC void bus_remove_timer(ux_loop_t *loop, ux_event_reminder_t* timer);

UX_FUNC ux_event_t* bus_dequeue(ux_loop_t *loop);
UX_FUNC ux_event_reminder_t* bus_timer_peek(ux_loop_t *loop, ux_clock_type type);
UX_FUNC ux_event_reminder_t* bus_timer_dequeue(ux_loop_t *loop, ux_clock_type type);

UX_FUNC ux_time_t bus_get_time(ux_loop_t *loop);
UX_FUNC int bus_set_time(ux_loop_t *loop, ux_time_t time);

UX_FUNC ux_time_t bus_get_exchange_time(ux_loop_t *loop);
UX_FUNC int bus_set_exchange_time(ux_loop_t *loop, ux_time_t time);

/* thread safe */
UX_EXTERN void ux_async_post(ux_loop_t *loop, ux_async_cb async_cb, void *data);

/* dispatch module */

UX_FUNC event_dispatch event_get_dispatcher(ux_event_t *e);

UX_FUNC void event_reminder_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void event_ask_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void event_bid_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void event_trade_dispatch(ux_loop_t *loop, ux_event_t *e);


#ifdef __cplusplus
}
#endif

#endif // __UX_INTERNAL_H__
