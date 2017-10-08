#ifndef __UX_INTERNAL_H__
#define __UX_INTERNAL_H__

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <uv.h>
#include <ux.h>
#include <assert.h>
#include "ux_atomic.h"
#include "ux_common.h"
#include "uthash.h"

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

/* bar factory */
typedef struct ux_barfactory_s ux_barfactory_t;
typedef struct ux_barfactory_item_s ux_barfactory_item_t;

typedef void (*ux_on_tick_cb)(ux_barfactory_item_t *item, ux_event_tick_t *tick);

#define UX_BAR_ITEM_PUBLIC_FIELDS \
    void* queue_node[2];   \
    ux_barfactory_t* factory;   \
    ux_instrument_t* instrument;    \
    ux_bar_input bar_input; \
    ux_on_tick_cb on_tick;  \
    ux_event_bar_t *bar;

#define UX_BAR_ITEM_COMMON_FILEDS \
    int provider_id;    \
    ux_bar_type bar_type;   \
    long bar_size;  \
    int session_enabled;    \
    ux_timespan_t session1; \
    ux_timespan_t session2;

struct ux_barfactory_item_s {
    UX_BAR_ITEM_PUBLIC_FIELDS
};

UX_FUNC void ux_barfactory_item_init(ux_barfactory_item_t *item);
UX_FUNC void ux_barfactory_item_destory(ux_barfactory_item_t *item);

typedef struct {
    UT_hash_handle hh;
    int instrument_id;
    void* queue[2];
} item_list_t;

struct ux_barfactory_s {
    ux_loop_t *framework;
    item_list_t* list_by_instrument_id;
    void* reminder_items;
};

UX_FUNC void ux_barfactory_init(ux_barfactory_t *factory);
UX_FUNC void ux_barfactory_destory(ux_barfactory_t *factory);
UX_EXTERN void ux_barfactory_add_item(ux_barfactory_t* factory, ux_barfactory_item_t* item);
UX_EXTERN void ux_barfactory_remove_item(ux_barfactory_t* factory, ux_barfactory_item_t* item);
UX_EXTERN void ux_barfactory_add_reminder(ux_barfactory_t *factory, ux_event_reminder_t *reminder);
UX_EXTERN void bar_factory_process_tick(ux_barfactory_t* factory, ux_event_tick_t* e);
UX_EXTERN void ux_barfactory_emit_bar_open(ux_barfactory_t *factory, ux_barfactory_item_t *item);
UX_EXTERN void ux_barfactory_emit_bar(ux_barfactory_t *factory, ux_barfactory_item_t *item);

typedef struct time_bar_item_s {
    UX_BAR_ITEM_PUBLIC_FIELDS
    UX_BAR_ITEM_COMMON_FILEDS
    ux_clock_type clock_type;
}time_bar_item_t;

UX_FUNC void time_bar_item_init(time_bar_item_t *item);
UX_FUNC void time_bar_item_destory(time_bar_item_t *item);

/* data manager module */
typedef struct data_manager_s {

}data_manager_t;

ux_event_ask_t *data_manager_get_ask(data_manager_t *manager, int instrument_id);
ux_event_ask_t *data_manager_get_bid(data_manager_t *manager, int instrument_id);
ux_event_ask_t *data_manager_get_trade(data_manager_t *manager, int instrument_id);

/* loop module */
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
    spscq_t buffed_event_queue;
    min_heap queue_heap[UX_CATEGORY_LAST];
    min_heap timer_heap[UX_CLOCK_LAST];
    uint64_t counter;
    ux_time_t time[UX_CLOCK_LAST];
    ux_event_t *saved_event;
    ux_queue_t* attached[128];
    int attached_count;
    int is_simulator_stop;
    /* stat */
    uint64_t event_count;
    /* bar factory */
    ux_barfactory_t bar_factory;
    data_manager_t data_manager;
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
typedef enum {
    UX_DISPATCH_LATE = 0,
    UX_DISPATCH_IMMEDIATELY
}ux_dispatch_mode;

UX_EXTERN void ux_dispatch_event(ux_loop_t* loop, ux_event_t* e, ux_dispatch_mode mode);
UX_FUNC   void ux_emit_bufferd_events(ux_loop_t *loop);

/* default event dispatch */
UX_FUNC void event_reminder_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void event_ask_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void event_bid_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void event_trade_dispatch(ux_loop_t *loop, ux_event_t *e);

#ifdef __cplusplus
}
#endif

#endif // __UX_INTERNAL_H__
