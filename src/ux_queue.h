#ifndef __UX_EVENT_QUEUE_H__
#define __UX_EVENT_QUEUE_H__

#include <stddef.h>
#include <stdint.h>
#include "ux_common.h"
#include "ux_spscq.h"
#include "ux_mpscq.h"

typedef struct ux_queue_s ux_queue_t;

typedef void (*ux_queue_event_handle)(ux_queue_t* q);

struct ux_queue_s {
    void *data;
    /* private */
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

ux_queue_t* ux_queue_init(ux_queue_t* q, unsigned int size, ux_queue_category category);

void ux_queue_destory(ux_queue_t *q);

void ux_queue_free(ux_queue_t *q);

static inline unsigned int ux_queue_size(ux_queue_t *q)
{
    return spsc_queue_size(&q->spsc);
}

static inline unsigned int ux_queue_capacity(ux_queue_t *q)
{
    return spsc_queue_capacity(&q->spsc);
}

static inline void *ux_queue_pop(ux_queue_t *q)
{
    return spsc_queue_pop(&q->spsc);
}

static inline void *ux_queue_peek(const ux_queue_t *q)
{
    return spsc_queue_peek(&q->spsc);
}

static inline int ux_queue_is_empty(ux_queue_t *q)
{
    return spsc_queue_is_empty(&q->spsc);
}

static inline int ux_queue_is_full(ux_queue_t *q)
{
    return spsc_queue_is_full(&q->spsc);
}

int ux_queue_push(ux_queue_t *q, void *e);

ux_queue_event_handle ux_queue_set_on_event(ux_queue_t *q, ux_queue_event_handle on_event);

//extern void _queue_on_event_callback(ux_queue_t *q, int flag);


#endif // __UX_EVENT_QUEUE_H__
