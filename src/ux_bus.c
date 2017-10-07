/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_internal.h"
#include "heap-inl.h"
#include <stdio.h>

#define LOG_OUT_OF_ORDER(x, y) printf("outoforder\n")

static int queue_less_than(const struct heap_node* ha, const struct heap_node* hb)
{
    UX_ASSERT(ha != NULL);
    UX_ASSERT(hb != NULL);

    const ux_queue_t* qa = container_of(ha, ux_queue_t, heap_node);
    const ux_queue_t* qb = container_of(hb, ux_queue_t, heap_node);

    ux_event_t* a = ux_queue_peek(qa);
    ux_event_t* b = ux_queue_peek(qb);

    UX_ASSERT(a != NULL);
    UX_ASSERT(b != NULL);

    if (a->timestamp < b->timestamp)
        return 1;
    if (b->timestamp < a->timestamp)
        return 0;

    if (qa->start_id < qb->start_id)
        return 1;
    if (qb->start_id < qa->start_id)
        return 0;

    return 0;
}

static int timer_less_than(const struct heap_node* ha,
    const struct heap_node* hb)
{
    UX_ASSERT(ha != NULL);
    UX_ASSERT(hb != NULL);

    const ux_event_reminder_t* a;
    const ux_event_reminder_t* b;

    a = container_of(ha, ux_event_reminder_t, heap_node);
    b = container_of(hb, ux_event_reminder_t, heap_node);

    if (a->stop < b->stop)
        return 1;
    if (b->stop < a->stop)
        return 0;

    if (a->start_id < b->start_id)
        return 1;
    if (b->start_id < a->start_id)
        return 0;

    return 0;
}

static UX_AINLINE int event_is_tick(ux_event_t* e)
{
    UX_ASSERT(e != NULL);

    ux_event_type type = e->type;
    if (type == UX_EVENT_ASK || type == UX_EVENT_BID || type == UX_EVENT_TRADE)
        return 1;
    else
        return 0;
}

static UX_AINLINE int heap_is_empty(min_heap* heap)
{
    UX_ASSERT(heap != NULL);

    return heap_min((const struct heap*)heap) == NULL;
}

static ux_event_t* event_pop_form_pipe(ux_loop_t *loop, ux_queue_category category)
{
    UX_ASSERT(loop != NULL);

    struct heap_node* node = heap_min((const struct heap*)&loop->queue_heap[category]);

    if (node == NULL)
        return NULL;

    ux_queue_t* q = container_of(node, ux_queue_t, heap_node);

    heap_remove((struct heap*)&loop->queue_heap[category],
        (struct heap_node*)node,
        queue_less_than);

    ux_event_t* e = ux_queue_pop(q);

    if (!ux_queue_is_empty(q)) {
        ux_loop_t *loop = q->loop;
        q->start_id = loop->counter++;
        heap_insert((struct heap*)&loop->queue_heap[category],
            (struct heap_node*)node,
            queue_less_than);
    }
    return e;
}

static UX_INLINE ux_event_reminder_t* timer_heap_peek(min_heap* heap)
{
    struct heap_node* node = heap_min((const struct heap*)heap);

    if (node == NULL)
        return NULL;

    ux_event_reminder_t* timer = container_of(node, ux_event_reminder_t, heap_node);

    return timer;
}

static ux_event_reminder_t* timer_heap_pop(min_heap* heap)
{
    struct heap_node* node = heap_min((const struct heap*)heap);

    if (node == NULL)
        return NULL;

    ux_event_reminder_t* timer = container_of(node, ux_event_reminder_t, heap_node);

    heap_remove((struct heap*)heap,
        (struct heap_node*)node,
        timer_less_than);

    if (timer->repeat != 0) {
        ux_loop_t *loop = timer->loop;
        timer->start_id = loop->counter++;
        timer->stop = timer->stop + timer->repeat;
        heap_insert((struct heap*)heap,
            (struct heap_node*)node,
            timer_less_than);
    }
    return timer;
}

static UX_INLINE int pipe_is_empty(ux_loop_t *loop, ux_queue_category category)
{
    return heap_is_empty(&loop->queue_heap[category]);
}

static void queue_heap_clear(min_heap* heap)
{
    struct heap_node* node;
    while ((node = heap_min((const struct heap*)heap)) != NULL) {
        ux_queue_t* q = container_of(node, ux_queue_t, heap_node);
        heap_remove((struct heap*)heap,
            (struct heap_node*)node,
            queue_less_than);
        ux_queue_unref(q);
    }
}

static void timer_heap_clear(min_heap* heap)
{
    struct heap_node* node;
    while ((node = heap_min((const struct heap*)heap)) != NULL) {
        ux_event_reminder_t* timer = container_of(node, ux_event_reminder_t, heap_node);
        heap_remove((struct heap*)heap,
            (struct heap_node*)node,
            timer_less_than);
        ux_event_unref((ux_event_t*)timer);
    }
}

static ux_event_t* bus_simualtion_dequeue(ux_loop_t *loop)
{
again:
    // market data
    if (!pipe_is_empty(loop, UX_CATEGORY_MARKET) && loop->saved_event == NULL) {
        ux_event_t* e = event_pop_form_pipe(loop, UX_CATEGORY_MARKET);

        //if (e == NULL) goto next; // TODO
        UX_ASSERT(e != NULL);

        if (e->type == UX_EVENT_SIMULATOR_STOP) {
            loop->is_simulator_stop = 1;
            goto again;
        }

        ux_time_t bus_time = bus_get_time(loop);
        if (e->timestamp < bus_time) {
            if (e->type != UX_EVENT_QUEUE_CLOSED && e->type != UX_EVENT_QUEUE_OPENED) {
                if (e->type != UX_EVENT_SIMULATOR_PROGRESS) {
                    LOG_OUT_OF_ORDER(loop_time, e);
                    goto again;
                }
            }
            e->timestamp = bus_get_time(loop);
        }
        loop->saved_event = e;
    }

next:
    // local clock timer
    if (!heap_is_empty(&loop->timer_heap[UX_CLOCK_LOCAL])) {
        if (loop->saved_event != NULL) {
            ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&loop->timer_heap[UX_CLOCK_LOCAL]);
            if (r->stop <= loop->saved_event->timestamp) {
                return (ux_event_t*)timer_heap_pop(&loop->timer_heap[UX_CLOCK_LOCAL]);
            }
        }
    }

    // exchage clock timer
    if (!heap_is_empty(&loop->timer_heap[UX_CLOCK_EXCHANGE])
        && loop->saved_event != NULL && event_is_tick(loop->saved_event)) {
        ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&loop->timer_heap[UX_CLOCK_EXCHANGE]);
        if (r->stop <= loop->saved_event->timestamp) {
            return (ux_event_t*)timer_heap_pop(&loop->timer_heap[UX_CLOCK_EXCHANGE]);
        }
    }

    // execution
    if (!pipe_is_empty(loop, UX_CATEGORY_EXECUTION)) {
        return event_pop_form_pipe(loop, UX_CATEGORY_EXECUTION);
    }

    // service
    if (!pipe_is_empty(loop, UX_CATEGORY_SERVICE)) {
        return event_pop_form_pipe(loop, UX_CATEGORY_SERVICE);
    }

    // command
    if (!pipe_is_empty(loop, UX_CATEGORY_COMMAND)) {
        return event_pop_form_pipe(loop, UX_CATEGORY_COMMAND);
    }

    if (loop->saved_event) {
        ux_event_t* e = loop->saved_event;
        loop->saved_event = NULL;
        for (int i = 0; i < loop->attached_count; i++) {
            if (e->type != UX_EVENT_QUEUE_OPENED && e->type != UX_EVENT_QUEUE_CLOSED) {
                ux_queue_t* q = loop->attached[i];
                ux_event_ref(e);
                ux_queue_push(q, e);
            }
        }
        return e;
    }

    return NULL;
}

static ux_event_t* bus_realtime_dequeue(ux_loop_t *loop)
{
    // market data
    if (!pipe_is_empty(loop, UX_CATEGORY_MARKET) && loop->saved_event == NULL) {
        loop->saved_event = event_pop_form_pipe(loop, UX_CATEGORY_MARKET);
    }

    // local clock timer
    if (!heap_is_empty(&loop->timer_heap[UX_CLOCK_LOCAL])) {
        ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&loop->timer_heap[UX_CLOCK_LOCAL]);
        if (r->stop <= bus_get_time(loop)) {
            return (ux_event_t*)timer_heap_pop(&loop->timer_heap[UX_CLOCK_LOCAL]);
        }
    }

    // exchage clock timer
    if (!heap_is_empty(&loop->timer_heap[UX_CLOCK_EXCHANGE]) && loop->saved_event != NULL
        && event_is_tick(loop->saved_event)) {
        ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&loop->timer_heap[UX_CLOCK_EXCHANGE]);
        if (r->stop <= loop->saved_event->timestamp) {
            return (ux_event_t*)timer_heap_pop(&loop->timer_heap[UX_CLOCK_EXCHANGE]);
        }
    }

    // execution
    if (!pipe_is_empty(loop, UX_CATEGORY_EXECUTION)) {
        return event_pop_form_pipe(loop, UX_CATEGORY_EXECUTION);
    }

    // command
    if (!pipe_is_empty(loop, UX_CATEGORY_COMMAND)) {
        return event_pop_form_pipe(loop, UX_CATEGORY_COMMAND);
    }

    // service
    if (!pipe_is_empty(loop, UX_CATEGORY_SERVICE)) {
        return event_pop_form_pipe(loop, UX_CATEGORY_SERVICE);
    }

    if (loop->saved_event) {
        ux_event_t* e = loop->saved_event;
        loop->saved_event = NULL;
        return e;
    }

    return NULL;
}

static inline void bus_add_pending_queue(ux_loop_t *loop)
{
    mpscq_node* node;
    while ((node = mpscq_pop(&loop->pending_queue))) {
        ux_queue_t* q = container_of(node, ux_queue_t, mpsc_node);
        bus_add_queue(loop, q);
    }
}

ux_event_t* bus_dequeue(ux_loop_t *loop)
{
    bus_add_pending_queue(loop);
    if (loop->mode == UX_BUS_SIMULATION)
        return bus_simualtion_dequeue(loop);
    else
        return bus_realtime_dequeue(loop);
}

ux_event_reminder_t* bus_timer_peek(ux_loop_t *loop, ux_clock_type type)
{
    assert(type < UX_CLOCK_LAST);
    return timer_heap_peek(&loop->timer_heap[type]);
}

ux_event_reminder_t* bus_timer_dequeue(ux_loop_t *loop, ux_clock_type type)
{
    assert(type < UX_CLOCK_LAST);
    return timer_heap_pop(&loop->timer_heap[type]);
}

void bus_attach(ux_loop_t* src, ux_loop_t* dst)
{
    int num = src->attached_count++;
    src->attached[num] = ux_zalloc(sizeof(ux_queue_t));
    if (src->attached[num] != NULL) {
        ux_queue_init(src->attached[num], 10000, UX_CATEGORY_MARKET);
        bus_add_queue(dst, src->attached[num]);
    }
}

void bus_detach(ux_loop_t* src, ux_loop_t* dst)
{
    for (int i = 0; i < src->attached_count; i++) {
        ux_queue_t* q = src->attached[i];
        UX_ASSERT(q != NULL);
        if (q->loop == dst) {
            bus_remove_queue(dst, q); // todo: add test(remove q from heap when q not insert heap)
            for (int j = i; j < src->attached_count - 1; j++)
                src->attached[j] = src->attached[j + 1];
            src->attached_count--;
        }
    }
}

void bus_add_queue(ux_loop_t *loop, ux_queue_t* q)
{
    UX_ASSERT(loop != NULL);
    UX_ASSERT(q != NULL);

    UX_ASSERT(q->loop == loop || q->loop == NULL);
    UX_ASSERT(q->category >= 0 && q->category < UX_CATEGORY_LAST);

    q->loop = loop;
    q->start_id = loop->counter++;

    if (ux_queue_is_empty(q))
        return;

    heap_insert((struct heap*)&loop->queue_heap[q->category],
        (struct heap_node*)&q->heap_node,
        queue_less_than);
}

void bus_remove_queue(ux_loop_t *loop, ux_queue_t* q)
{
    UX_ASSERT(loop != NULL);
    UX_ASSERT(q != NULL);

    heap_remove((struct heap*)&loop->queue_heap[q->category],
        (struct heap_node*)&q->heap_node,
        queue_less_than);
}

void bus_add_timer(ux_loop_t *loop, ux_event_reminder_t* timer)
{
    UX_ASSERT(loop != NULL);
    UX_ASSERT(timer != NULL);

    timer->loop = loop;
    timer->start_id = loop->counter++;
    heap_insert((struct heap*)&loop->timer_heap[timer->clock_type],
        (struct heap_node*)&timer->heap_node,
        timer_less_than);
}

void bus_remove_timer(ux_loop_t *loop, ux_event_reminder_t* timer)
{
    UX_ASSERT(loop != NULL);
    UX_ASSERT(timer != NULL);

    heap_remove((struct heap*)&loop->timer_heap[timer->clock_type],
        (struct heap_node*)&timer->heap_node,
        timer_less_than);
}

ux_time_t bus_get_time(ux_loop_t *loop)
{
    UX_ASSERT(loop != NULL);

    if (loop->mode == UX_BUS_SIMULATION)
        return loop->time[UX_CLOCK_LOCAL];
    else
        return ux_time_now();
}

int bus_set_time(ux_loop_t *loop, ux_time_t time)
{
    UX_ASSERT(loop != NULL);

    if (loop->mode == UX_BUS_REALTIME)
        return -1;

    if (time < loop->time[UX_CLOCK_LOCAL]) {
        return -1;
    }
    loop->time[UX_CLOCK_LOCAL] = time;
    return 0;
}

ux_time_t bus_get_exchange_time(ux_loop_t *loop)
{
    UX_ASSERT(loop != NULL);

    return loop->time[UX_CLOCK_EXCHANGE];
}

int bus_set_exchange_time(ux_loop_t *loop, ux_time_t time)
{
    UX_ASSERT(loop != NULL);

    if (time < loop->time[UX_CLOCK_EXCHANGE]) {
        return -1;
    }
    loop->time[UX_CLOCK_EXCHANGE] = time;
    return 0;
}

void ux_loop_init(ux_loop_t *loop)
{
    uv_mutex_init(&loop->wait_mutex);
    uv_cond_init(&loop->wait_cond);
    loop->stop_flag = 0;
    mpscq_init(&loop->async_queue);

    /* bus init */
    loop->mode = UX_BUS_SIMULATION;

    mpscq_init(&loop->pending_queue);
    for (int i = 0; i < UX_CATEGORY_LAST; i++)
        heap_init((struct heap*)&loop->queue_heap[i]);

    heap_init((struct heap*)&loop->timer_heap[UX_CLOCK_LOCAL]);
    heap_init((struct heap*)&loop->timer_heap[UX_CLOCK_EXCHANGE]);

    loop->time[UX_CLOCK_LOCAL] = MIN_DATE_TIME;
    loop->time[UX_CLOCK_EXCHANGE] = MIN_DATE_TIME;

    loop->counter = 1;
    loop->saved_event = NULL;
    loop->is_simulator_stop = 0;

    memset(&loop->attached, '\0', sizeof(loop->attached));
    loop->attached_count = 0;
}

void ux_loop_destory(ux_loop_t *loop)
{
    uv_cond_destroy(&loop->wait_cond);
    uv_mutex_destroy(&loop->wait_mutex);
    ux_loop_clear(loop);
}


void ux_loop_clear(ux_loop_t *loop)
{
    if (loop->saved_event != NULL) {
        ux_event_unref(loop->saved_event);
        loop->saved_event = NULL;
    }

    for (int i = 0; i < UX_CATEGORY_LAST; i++)
        queue_heap_clear(&loop->queue_heap[i]);

    for (int i = 0; i < UX_CLOCK_LAST; i++)
        timer_heap_clear(&loop->timer_heap[i]);

    for (int i = 0; i < loop->attached_count; i++) {
        ux_queue_t* q = loop->attached[i];
        bus_detach(loop, q->loop);
        ux_queue_destory(q);
    }
    loop->attached_count = 0;
}
