/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_bus.h"
#include "heap-inl.h"
#include "ux_common.h"
#include "ux_datetime.h"
#include "ux_event.h"
#include "ux_queue.h"
#include <assert.h>
#include <stdio.h>

#define LOG_OUT_OF_ORDER(x, y) printf("outoforder\n")

static int queue_less_than(const struct heap_node* ha, const struct heap_node* hb)
{
    assert(ha != NULL);
    assert(hb != NULL);

    const ux_queue_t* qa = container_of(ha, ux_queue_t, heap_node);
    const ux_queue_t* qb = container_of(hb, ux_queue_t, heap_node);

    ux_event_t* a = ux_queue_peek(qa);
    ux_event_t* b = ux_queue_peek(qb);

    assert(a != NULL);
    assert(b != NULL);

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
    assert(ha != NULL);
    assert(hb != NULL);

    const ux_event_reminder_t* a;
    const ux_event_reminder_t* b;

    a = container_of(ha, ux_event_reminder_t, heap_node);
    b = container_of(hb, ux_event_reminder_t, heap_node);

    if (a->timeout < b->timeout)
        return 1;
    if (b->timeout < a->timeout)
        return 0;

    if (a->start_id < b->start_id)
        return 1;
    if (b->start_id < a->start_id)
        return 0;

    return 0;
}

static UX_AINLINE int event_is_tick(ux_event_t* e)
{
    ux_event_type type = e->type;
    if (type == UX_EVENT_ASK || type == UX_EVENT_BID || type == UX_EVENT_TRADE)
        return 1;
    else
        return 0;
}

static UX_AINLINE int heap_is_empty(min_heap* heap)
{
    return heap_min((const struct heap*)heap) == NULL;
}

static UX_INLINE ux_queue_t* queue_heap_peek(min_heap* heap)
{
    struct heap_node* node = heap_min((const struct heap*)heap);

    if (node == NULL)
        return NULL;

    ux_queue_t* q = container_of(node, ux_queue_t, heap_node);

    return q;
}

static ux_event_t* queue_heap_pop_event(min_heap* heap)
{
    struct heap_node* node = heap_min((const struct heap*)heap);

    if (node == NULL)
        return NULL;

    ux_queue_t* q = container_of(node, ux_queue_t, heap_node);

    heap_remove((struct heap*)heap,
        (struct heap_node*)node,
        queue_less_than);

    ux_event_t* e = ux_queue_pop(q);

    if (!ux_queue_is_empty(q)) {
        ux_bus_t* bus = q->bus;
        q->start_id = bus->counter++;
        heap_insert((struct heap*)heap,
            (struct heap_node*)node,
            queue_less_than);
    }
    return e;
}

static UX_INLINE ux_event_t* event_pop_form_pipe(ux_bus_t* bus, ux_queue_category category)
{
    return queue_heap_pop_event(&bus->queue_heap[category]);
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
        ux_bus_t* bus = timer->bus;
        timer->start_id = bus->counter++;
        heap_insert((struct heap*)heap,
            (struct heap_node*)node,
            timer_less_than);
    }
    return timer;
}

static UX_INLINE int pipe_is_empty(ux_bus_t* bus, ux_queue_category category)
{
    return heap_is_empty(&bus->queue_heap[category]);
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

static ux_event_t* bus_simualtion_dequeue(ux_bus_t* bus)
{
again:
    // market data
    if (!pipe_is_empty(bus, UX_CATEGORY_MARKET) && bus->saved_event == NULL) {
        ux_event_t* e = event_pop_form_pipe(bus, UX_CATEGORY_MARKET);

        //if (e == NULL) goto next; // TODO
        UX_ASSERT( e != NULL);

        if (e->type == UX_EVENT_SIMULATOR_STOP) {
            bus->is_simulator_stop = 1;
            goto again;
        }

        datetime_t bus_time = ux_bus_get_time(bus);
        if (e->timestamp < bus_time) {
            if (e->type != UX_EVENT_QUEUE_CLOSED && e->type != UX_EVENT_QUEUE_OPENED) {
                if (e->type != UX_EVENT_SIMULATOR_PROGRESS) {
                    LOG_OUT_OF_ORDER(bus_time, e);
                    goto again;
                }
            }
            e->timestamp = ux_bus_get_time(bus);
        }
        bus->saved_event = e;
    }

    next:
    // local clock
    if (!heap_is_empty(&bus->timer_heap[UX_CLOCK_LOCAL])) {
        if (bus->saved_event != NULL) {
            ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&bus->timer_heap[UX_CLOCK_LOCAL]);
            if (r->timestamp <= bus->saved_event->timestamp) {
                return (ux_event_t*)timer_heap_pop(&bus->timer_heap[UX_CLOCK_LOCAL]);
            }
        }
    }

    // exchage clock
    if (!heap_is_empty(&bus->timer_heap[UX_CLOCK_EXCHANGE])
        && bus->saved_event != NULL && event_is_tick(bus->saved_event)) {
        ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&bus->timer_heap[UX_CLOCK_EXCHANGE]);
        if (r->timestamp <= bus->saved_event->timestamp) {
            return (ux_event_t*)timer_heap_pop(&bus->timer_heap[UX_CLOCK_EXCHANGE]);
        }
    }

    // execution
    if (!pipe_is_empty(bus, UX_CATEGORY_EXECUTION)) {
        return event_pop_form_pipe(bus, UX_CATEGORY_EXECUTION);
    }

    // service
    if (!pipe_is_empty(bus, UX_CATEGORY_SERVICE)) {
        return event_pop_form_pipe(bus, UX_CATEGORY_SERVICE);
    }

    // command
    if (!pipe_is_empty(bus, UX_CATEGORY_COMMAND)) {
        return event_pop_form_pipe(bus, UX_CATEGORY_COMMAND);
    }

    if (bus->saved_event) {
        ux_event_t* e = bus->saved_event;
        bus->saved_event = NULL;
        for (int i = 0; i < bus->attached_count; i++) {
            if (e->type != UX_EVENT_QUEUE_OPENED && e->type != UX_EVENT_QUEUE_CLOSED) {
                ux_queue_t* q = bus->attached[i];
                ux_event_ref(e);
                ux_queue_push(q, e);
            }
        }
        return e;
    }

    return NULL;
}

static ux_event_t* bus_realtime_dequeue(ux_bus_t* bus)
{
    // market data
    if (!pipe_is_empty(bus, UX_CATEGORY_MARKET) && bus->saved_event == NULL) {
        bus->saved_event = event_pop_form_pipe(bus, UX_CATEGORY_MARKET);
    }

    // local clock
    if (!heap_is_empty(&bus->timer_heap[UX_CLOCK_LOCAL])) {
        ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&bus->timer_heap[UX_CLOCK_LOCAL]);
        if (r->timestamp <= ux_bus_get_time(bus)) {
            return (ux_event_t*)timer_heap_pop(&bus->timer_heap[UX_CLOCK_LOCAL]);
        }
    }

    // exchage clock
    if (!heap_is_empty(&bus->timer_heap[UX_CLOCK_EXCHANGE]) && bus->saved_event != NULL
        && event_is_tick(bus->saved_event)) {
        ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&bus->timer_heap[UX_CLOCK_EXCHANGE]);
        if (r->timestamp <= bus->saved_event->timestamp) {
            return (ux_event_t*)timer_heap_pop(&bus->timer_heap[UX_CLOCK_EXCHANGE]);
        }
    }

    // execution
    if (!pipe_is_empty(bus, UX_CATEGORY_EXECUTION)) {
        return event_pop_form_pipe(bus, UX_CATEGORY_EXECUTION);
    }

    // command
    if (!pipe_is_empty(bus, UX_CATEGORY_COMMAND)) {
        return event_pop_form_pipe(bus, UX_CATEGORY_COMMAND);
    }

    // service
    if (!pipe_is_empty(bus, UX_CATEGORY_SERVICE)) {
        return event_pop_form_pipe(bus, UX_CATEGORY_SERVICE);
    }

    if (bus->saved_event) {
        ux_event_t* e = bus->saved_event;
        bus->saved_event = NULL;
        return e;
    }

    return NULL;
}

ux_event_t* ux_bus_next_event(ux_bus_t* bus)
{
    if (bus->mode == UX_BUS_SIMULATION)
        return bus_simualtion_dequeue(bus);
    else
        return bus_realtime_dequeue(bus);
}

int64_t ux_bus_next_timeout(ux_bus_t* bus)
{
    ux_event_reminder_t* r = (ux_event_reminder_t*)timer_heap_peek(&bus->timer_heap[UX_CLOCK_LOCAL]);
    if (r)
        return r->timeout - datetime_now();
    else
        return 0;
}

UX_FUNC ux_event_reminder_t* ux_bus_next_reminder(ux_bus_t* bus, ux_clock_type type)
{
    void* heap = &bus->timer_heap[type];

    return timer_heap_pop(heap);
}

void ux_bus_attach(ux_bus_t* src, ux_bus_t* dst)
{
    int num = src->attached_count++;
    src->attached[num] = ux_zalloc(sizeof(ux_queue_t));
    if (src->attached[num] != NULL) {
        ux_queue_init(src->attached[num], 10000, UX_CATEGORY_MARKET);
        ux_bus_add_queue(dst, src->attached[num]);
    }
}

void ux_bus_detach(ux_bus_t* src, ux_bus_t* dst)
{
    for (int i = 0; i < src->attached_count; i++) {
        ux_queue_t* q = src->attached[i];
        assert(q != NULL);
        if (q->bus == dst) {
            ux_bus_remove_queue(dst, q); // todo: add test(remove q from heap when q not insert heap)
            for (int j = i; j < src->attached_count - 1; j++)
                src->attached[j] = src->attached[j + 1];
            src->attached_count--;
        }
    }
}

void ux_bus_init(ux_bus_t* bus, ux_bus_mode mode)
{
    bus->mode = mode;

    for (int i = 0; i < UX_CATEGORY_LAST; i++)
        heap_init((struct heap*)&bus->queue_heap[i]);

    for (int i = 0; i < UX_CLOCK_LAST; i++)
        heap_init((struct heap*)&bus->timer_heap[i]);

    bus->counter = 1;
    bus->saved_event = NULL;

    bus->time[UX_CLOCK_LOCAL] = MIN_DATE_TIME;
    bus->time[UX_CLOCK_EXCHANGE] = MIN_DATE_TIME;

    memset(&bus->attached, '\0', sizeof(bus->attached));
    bus->attached_count = 0;
    bus->is_simulator_stop = 0;
}

void ux_bus_destory(ux_bus_t* bus)
{
    ux_bus_clear(bus);
}

void ux_bus_add_queue(ux_bus_t* bus, void* data)
{
    UX_ASSERT(bus != NULL);
    UX_ASSERT(data != NULL);

    ux_queue_t* q = (ux_queue_t*)data;
    UX_ASSERT(q->bus == bus || q->bus == NULL);
    UX_ASSERT(q->category >= 0 && q->category < UX_CATEGORY_LAST);

    q->bus = bus;
    q->start_id = bus->counter++;

     if(ux_queue_is_empty(q)) return;
    heap_insert((struct heap*)&bus->queue_heap[q->category],
        (struct heap_node*)&q->heap_node,
        queue_less_than);
}

void ux_bus_remove_queue(ux_bus_t* bus, void* data)
{
    ux_queue_t* q = (ux_queue_t*)data;
    heap_remove((struct heap*)&bus->queue_heap[q->category],
        (struct heap_node*)&q->heap_node,
        queue_less_than);
}

void ux_bus_add_timer(ux_bus_t* bus, ux_event_reminder_t* timer)
{
    timer->bus = bus;
    timer->start_id = bus->counter++;
    heap_insert((struct heap*)&bus->timer_heap[timer->clock_type],
        (struct heap_node*)&timer->heap_node,
        timer_less_than);
}

void ux_bus_remove_timer(ux_bus_t* bus, ux_event_reminder_t* timer)
{
    heap_remove((struct heap*)&bus->timer_heap[timer->clock_type],
        (struct heap_node*)&timer->heap_node,
        timer_less_than);
}

datetime_t ux_bus_get_time(ux_bus_t* bus)
{
    if (bus->mode == UX_BUS_SIMULATION)
        return bus->time[UX_CLOCK_LOCAL];
    else
        return datetime_now();
}

int ux_bus_set_time(ux_bus_t* bus, datetime_t time)
{
    if (bus->mode == UX_BUS_REALTIME)
        return -1;

    if (time < bus->time[UX_CLOCK_LOCAL]) {
        return -1;
    }
    bus->time[UX_CLOCK_LOCAL] = time;
    return 0;
}

datetime_t ux_bus_get_exchange_time(ux_bus_t* bus)
{
    return bus->time[UX_CLOCK_EXCHANGE];
}

int ux_bus_set_exchange_time(ux_bus_t* bus, datetime_t time)
{
    if (time < bus->time[UX_CLOCK_EXCHANGE]) {
        return -1;
    }
    bus->time[UX_CLOCK_EXCHANGE] = time;
    return 0;
}

void ux_bus_clear(ux_bus_t* bus)
{
    if (bus->saved_event != NULL) {
        ux_event_unref(bus->saved_event);
        bus->saved_event = NULL;
    }

    for (int i = 0; i < UX_CATEGORY_LAST; i++)
        queue_heap_clear(&bus->queue_heap[i]);

    for (int i = 0; i < UX_CLOCK_LAST; i++)
        timer_heap_clear(&bus->timer_heap[i]);

    for (int i = 0; i < bus->attached_count; i++) {
        ux_queue_t* q = bus->attached[i];
        ux_bus_detach(bus, q->bus);
        ux_queue_destory(q);
    }
    bus->attached_count = 0;
}
