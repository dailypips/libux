#ifndef __UX_EVENT_QUEUE_H__
#define __UX_EVENT_QUEUE_H__

#include <stddef.h>
#include <stdint.h>
#include "ux_common.h"
#include "ux_spscq.h"
#include "ux_mpscq.h"

/*
 * ux_queue_t
 * 单生产者单消费者队列
 * 队列内容的生命周期管理
 * 队列并不管理内容的生命周期！！
 */
typedef struct ux_queue_s ux_queue_t;

typedef void (*ux_queue_event_handle)(ux_queue_t* q);

struct ux_queue_s {
    void *data;
    /* private */
    ux_atomic_t refcount;
    ux_spscq_t spsc;
    /* only access by consumer */
    void *bus;
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

UX_EXPORT int ux_queue_push(ux_queue_t *q, void *e);

//UX_EXPORT void ux_queue_close(ux_queue_t *q);

UX_FUNC unsigned int ux_queue_size(ux_queue_t *q);

UX_FUNC unsigned int ux_queue_capacity(ux_queue_t *q);

UX_FUNC void *ux_queue_pop(ux_queue_t *q);

UX_FUNC void *ux_queue_peek(const ux_queue_t *q);

UX_FUNC int ux_queue_is_empty(ux_queue_t *q);

UX_FUNC int ux_queue_is_full(ux_queue_t *q);

#endif // __UX_EVENT_QUEUE_H__
