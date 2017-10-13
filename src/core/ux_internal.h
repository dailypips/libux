#ifndef __UX_INTERNAL_H__
#define __UX_INTERNAL_H__

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <uv.h>
#include <ux/ux.h>
#include <assert.h>
#include "ux_atomic.h"
#include "ux_macro.h"
#include "ux_hash.h"

#ifdef __cplusplus
extern "C" {
#endif

/* event  module */

/* event class info */
typedef void (*event_destory)(ux_event_t* e);
typedef ux_event_t* (*event_clone)(ux_event_t* e);
typedef void (*event_dispatch)(ux_ctx_t *ctx, ux_event_t* e);

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
    void *ctx;
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
//typedef struct ux_barfactory_s ux_barfactory_t;
//typedef struct ux_barfactory_item_s ux_barfactory_item_t;

//typedef void (*ux_on_tick_cb)(ux_barfactory_item_t *item, ux_event_tick_t *tick);


UX_FUNC void ux_barfactory_item_init(ux_barfactory_item_t *item);
UX_FUNC void ux_barfactory_item_destory(ux_barfactory_item_t *item);

typedef struct {
    UT_hash_handle hh;
    int instrument_id;
    void* queue[2];
} item_list_t;

#define BAR_FACTORY_PUBLIC_FIELDS \
    item_list_t* list_by_instrument_id; \
    void* reminder_items;

struct ux_barfactory_s {
    BAR_FACTORY_PUBLIC_FIELDS
};

UX_FUNC void ux_barfactory_init(ux_barfactory_t *factory);
UX_FUNC void ux_barfactory_destory(ux_barfactory_t *factory);
UX_EXTERN void ux_barfactory_add_item(ux_barfactory_t* factory, ux_barfactory_item_t* item);
UX_EXTERN void ux_barfactory_remove_item(ux_barfactory_t* factory, ux_barfactory_item_t* item);
UX_EXTERN void ux_barfactory_add_reminder(ux_barfactory_t *factory, ux_event_reminder_t *reminder);
UX_EXTERN void bar_factory_process_tick(ux_barfactory_t* factory, ux_event_tick_t* e);
UX_EXTERN void ux_barfactory_emit_bar_open(ux_barfactory_t *factory, ux_barfactory_item_t *item);
UX_EXTERN void ux_barfactory_emit_bar(ux_barfactory_t *factory, ux_barfactory_item_t *item);

/* data manager module */
typedef struct tick_hash_node_s {
    UT_hash_handle hh;
    int key;
    ux_event_tick_t *e;
}tick_hash_node_t;

typedef struct data_manager_s {
    tick_hash_node_t *ask_hash;
    tick_hash_node_t *bid_hash;
    tick_hash_node_t *trade_hash;
}data_manager_t;

typedef struct instrument_hash_node_s {
    UT_hash_handle hh;
    int id;
    ux_instrument_t *instrument;
}instrument_hash_node_t;

typedef struct instrument_manager_s {
    instrument_hash_node_t *instrument_hash;
}instrument_manager_t;

ux_event_ask_t *data_manager_get_ask(data_manager_t *manager, int instrument_id);
ux_event_ask_t *data_manager_get_bid(data_manager_t *manager, int instrument_id);
ux_event_ask_t *data_manager_get_trade(data_manager_t *manager, int instrument_id);

/* ctx module */
typedef struct {
    void *min;
    unsigned int nelts;
}min_heap;

struct ux_ctx_s {
    uv_mutex_t wait_mutex;
    uv_cond_t  wait_cond;
    int stop_flag;
    mpscq_t async_queue;
    /* bus field */
    ux_bus_mode mode;
    mpscq_t pending_queue;      //保存要被添加到heap中的queue
    spscq_t buffed_event_queue; //保存被缓存的事件
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
    /* instrument manager*/

    /* bar factory */
    ux_barfactory_t bar_factory;
    //BAR_FACTORY_PUBLIC_FIELDS
    data_manager_t data_manager;

    /* simulator */
    ux_data_simulator_t *data_simulator;
    ux_execution_simulator_t *execution_simulator;
};

UX_FUNC void ux_ctx_init(ux_ctx_t *ctx);
UX_FUNC void ux_ctx_destory(ux_ctx_t *ctx);
UX_FUNC void ux_ctx_clear(ux_ctx_t *ctx);

UX_FUNC void bus_attach(ux_ctx_t* src, ux_ctx_t *dst);
UX_FUNC void bus_detach(ux_ctx_t* src, ux_ctx_t *dst);

UX_FUNC void bus_add_queue(ux_ctx_t *ctx, ux_queue_t *q);
UX_FUNC void bus_remove_queue(ux_ctx_t *ctx, ux_queue_t *q);

UX_FUNC void bus_add_timer(ux_ctx_t *ctx, ux_event_reminder_t* timer);
UX_FUNC void bus_remove_timer(ux_ctx_t *ctx, ux_event_reminder_t* timer);

UX_FUNC ux_event_t* bus_dequeue(ux_ctx_t *ctx);
UX_FUNC ux_event_reminder_t* bus_timer_peek(ux_ctx_t *ctx, ux_clock_type type);
UX_FUNC ux_event_reminder_t* bus_timer_dequeue(ux_ctx_t *ctx, ux_clock_type type);

UX_FUNC ux_time_t bus_get_time(ux_ctx_t *ctx);
UX_FUNC int bus_set_time(ux_ctx_t *ctx, ux_time_t time);

UX_FUNC ux_time_t bus_get_exchange_time(ux_ctx_t *ctx);
UX_FUNC int bus_set_exchange_time(ux_ctx_t *ctx, ux_time_t time);

/* thread safe */
UX_EXTERN void ux_async_post(ux_ctx_t *ctx, ux_async_cb async_cb, void *data);

/* dispatch module */
typedef enum {
    UX_DISPATCH_LATE = 0,
    UX_DISPATCH_IMMEDIATELY
}ux_dispatch_mode;

UX_EXTERN void ux_dispatch_event(ux_ctx_t *ctx, ux_event_t* e, ux_dispatch_mode mode);
UX_FUNC   void ux_emit_bufferd_events(ux_ctx_t *ctx);

/* default event dispatch */
UX_FUNC void event_reminder_dispatch(ux_ctx_t *ctx, ux_event_t *e);
UX_FUNC void event_ask_dispatch(ux_ctx_t *ctx, ux_event_t *e);
UX_FUNC void event_bid_dispatch(ux_ctx_t *ctx, ux_event_t *e);
UX_FUNC void event_trade_dispatch(ux_ctx_t *ctx, ux_event_t *e);


/* series & indicator */
typedef struct ux_series_s ux_series_t;

struct ux_series_s {
	void* indicators[2];
	const char* (*get_name)(ux_series_t *s);
	const char* (*get_description)(ux_series_t *s);
	int (*get_size)(ux_series_t *s);
	double (*get_first)(ux_series_t *s);
	double (*get_last)(ux_series_t *s);
	ux_time_t (*get_first_time)(ux_series_t *s);
	ux_time_t (*get_last_time)(ux_series_t *s);
	double (*get)(ux_series_t, int index);
	double (*get_min)(ux_series_t* s, ux_time_t start, ux_time_t end);
	double (*get_max)(ux_series_t* s, ux_time_t start, ux_time_t end);
};

typedef struct {
	ux_time_t timestamp;
	double value;
}ux_time_series_data_t;


#ifdef __cplusplus
}
#endif

#endif // __UX_INTERNAL_H__
