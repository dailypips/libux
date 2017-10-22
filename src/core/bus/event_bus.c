/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include <ux/base/ux_datetime.h>
#include <ux/base/ux_memory.h>
#include <ux/event/tick.h>
#include "event_queue.h"
#include "heap-inl.h"
#include <stdio.h>
#include "event_bus.h"
#include "context.h"

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

    const uxe_reminder_t* a;
    const uxe_reminder_t* b;

    a = container_of(ha, uxe_reminder_t, heap_node);
    b = container_of(hb, uxe_reminder_t, heap_node);

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
    UX_ASSERT(e != NULL);

    uxe_type type = e->type;
    if (type == UXE_ASK || type == UXE_BID || type == UXE_TRADE)
        return 1;
    else
        return 0;
}

static UX_AINLINE int heap_is_empty(min_heap* heap)
{
    UX_ASSERT(heap != NULL);

    return heap_min((const struct heap*)heap) == NULL;
}

static ux_event_t* event_pop_form_pipe(ux_ctx_t *ctx, ux_queue_category category)
{
    UX_ASSERT(ctx != NULL);

    struct heap_node* node = heap_min((const struct heap*)&ctx->queue_heap[category]);

    if (node == NULL)
        return NULL;

    ux_queue_t* q = container_of(node, ux_queue_t, heap_node);

    heap_remove((struct heap*)&ctx->queue_heap[category],
        (struct heap_node*)node,
        queue_less_than);

    ux_event_t* e = ux_queue_pop(q);

    if (!ux_queue_is_empty(q)) {
        ux_ctx_t *ctx = q->ctx;
        q->start_id = ctx->counter++;
        heap_insert((struct heap*)&ctx->queue_heap[category],
            (struct heap_node*)node,
            queue_less_than);
    }
    return e;
}

static UX_INLINE uxe_reminder_t* timer_heap_peek(min_heap* heap)
{
    struct heap_node* node = heap_min((const struct heap*)heap);

    if (node == NULL)
        return NULL;

    uxe_reminder_t* timer = container_of(node, uxe_reminder_t, heap_node);

    return timer;
}

static uxe_reminder_t* timer_heap_pop(min_heap* heap)
{
    struct heap_node* node = heap_min((const struct heap*)heap);

    if (node == NULL)
        return NULL;

    uxe_reminder_t* timer = container_of(node, uxe_reminder_t, heap_node);

    heap_remove((struct heap*)heap,
        (struct heap_node*)node,
        timer_less_than);

    if (timer->repeat != 0) {
        ux_ctx_t *ctx = timer->ctx;
        timer->start_id = ctx->counter++;
        timer->timeout = timer->timeout + timer->repeat;
        heap_insert((struct heap*)heap,
            (struct heap_node*)node,
            timer_less_than);
    }
    return timer;
}

static UX_INLINE int pipe_is_empty(ux_ctx_t *ctx, ux_queue_category category)
{
    return heap_is_empty(&ctx->queue_heap[category]);
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
        uxe_reminder_t* timer = container_of(node, uxe_reminder_t, heap_node);
        heap_remove((struct heap*)heap,
            (struct heap_node*)node,
            timer_less_than);
        ux_event_unref((ux_event_t*)timer);
    }
}

static ux_event_t* bus_simualtion_dequeue(ux_ctx_t *ctx)
{
again:
    // market data
    if (!pipe_is_empty(ctx, UX_CATEGORY_MARKET) && ctx->saved_event == NULL) {
        ux_event_t* e = event_pop_form_pipe(ctx, UX_CATEGORY_MARKET);

        //if (e == NULL) goto next; // TODO
        UX_ASSERT(e != NULL);

        if (e->type == UXE_SIMULATOR_STOP) {
            ctx->is_simulator_stop = 1;
            goto again;
        }

        ux_time_t bus_time = bus_get_time(ctx);
        if (e->timestamp < bus_time) {
            if (e->type != UXE_QUEUE_CLOSED && e->type != UXE_QUEUE_OPENED) {
                if (e->type != UXE_SIMULATOR_PROGRESS) {
                    LOG_OUT_OF_ORDER(ctx_time, e);
                    goto again;
                }
            }
            e->timestamp = bus_get_time(ctx);
        }
        ctx->saved_event = e;
    }

next:
    // local clock timer
    if (!heap_is_empty(&ctx->timer_heap[UX_CLOCK_LOCAL])) {
        if (ctx->saved_event != NULL) {
            uxe_reminder_t* r = (uxe_reminder_t*)timer_heap_peek(&ctx->timer_heap[UX_CLOCK_LOCAL]);
            if (r->timeout <= ctx->saved_event->timestamp) {
                return (ux_event_t*)timer_heap_pop(&ctx->timer_heap[UX_CLOCK_LOCAL]);
            }
        }
    }

    // exchage clock timer
    if (!heap_is_empty(&ctx->timer_heap[UX_CLOCK_EXCHANGE])
        && ctx->saved_event != NULL && event_is_tick(ctx->saved_event)) {
        uxe_reminder_t* r = (uxe_reminder_t*)timer_heap_peek(&ctx->timer_heap[UX_CLOCK_EXCHANGE]);
        if (r->timeout <= ctx->saved_event->timestamp) {
            return (ux_event_t*)timer_heap_pop(&ctx->timer_heap[UX_CLOCK_EXCHANGE]);
        }
    }

    // execution
    if (!pipe_is_empty(ctx, UX_CATEGORY_EXECUTION)) {
        return event_pop_form_pipe(ctx, UX_CATEGORY_EXECUTION);
    }

    // service
    if (!pipe_is_empty(ctx, UX_CATEGORY_SERVICE)) {
        return event_pop_form_pipe(ctx, UX_CATEGORY_SERVICE);
    }

    // command
    if (!pipe_is_empty(ctx, UX_CATEGORY_COMMAND)) {
        return event_pop_form_pipe(ctx, UX_CATEGORY_COMMAND);
    }

    if (ctx->saved_event) {
        ux_event_t* e = ctx->saved_event;
        ctx->saved_event = NULL;
        for (int i = 0; i < ctx->attached_count; i++) {
            if (e->type != UXE_QUEUE_OPENED && e->type != UXE_QUEUE_CLOSED) {
                ux_queue_t* q = ctx->attached[i];
                ux_event_ref(e);
                ux_queue_push(q, e);
            }
        }
        return e;
    }

    return NULL;
}

static ux_event_t* bus_realtime_dequeue(ux_ctx_t *ctx)
{
    // market data
    if (!pipe_is_empty(ctx, UX_CATEGORY_MARKET) && ctx->saved_event == NULL) {
        ctx->saved_event = event_pop_form_pipe(ctx, UX_CATEGORY_MARKET);
    }

    // local clock timer
    if (!heap_is_empty(&ctx->timer_heap[UX_CLOCK_LOCAL])) {
        uxe_reminder_t* r = (uxe_reminder_t*)timer_heap_peek(&ctx->timer_heap[UX_CLOCK_LOCAL]);
        if (r->timeout <= bus_get_time(ctx)) {
            return (ux_event_t*)timer_heap_pop(&ctx->timer_heap[UX_CLOCK_LOCAL]);
        }
    }

    // exchage clock timer
    if (!heap_is_empty(&ctx->timer_heap[UX_CLOCK_EXCHANGE]) && ctx->saved_event != NULL
        && event_is_tick(ctx->saved_event)) {
        uxe_reminder_t* r = (uxe_reminder_t*)timer_heap_peek(&ctx->timer_heap[UX_CLOCK_EXCHANGE]);
        if (r->timeout <= ctx->saved_event->timestamp) {
            return (ux_event_t*)timer_heap_pop(&ctx->timer_heap[UX_CLOCK_EXCHANGE]);
        }
    }

    // execution
    if (!pipe_is_empty(ctx, UX_CATEGORY_EXECUTION)) {
        return event_pop_form_pipe(ctx, UX_CATEGORY_EXECUTION);
    }

    // command
    if (!pipe_is_empty(ctx, UX_CATEGORY_COMMAND)) {
        return event_pop_form_pipe(ctx, UX_CATEGORY_COMMAND);
    }

    // service
    if (!pipe_is_empty(ctx, UX_CATEGORY_SERVICE)) {
        return event_pop_form_pipe(ctx, UX_CATEGORY_SERVICE);
    }

    if (ctx->saved_event) {
        ux_event_t* e = ctx->saved_event;
        ctx->saved_event = NULL;
        return e;
    }

    return NULL;
}

static inline void bus_add_pending_queue(ux_ctx_t *ctx)
{
    mpscq_node* node;
    while ((node = mpscq_pop(&ctx->pending_queue))) {
        ux_queue_t* q = container_of(node, ux_queue_t, mpsc_node);
        bus_add_queue(ctx, q);
    }
}

ux_event_t* bus_dequeue(ux_ctx_t *ctx)
{
    bus_add_pending_queue(ctx);
    if (ctx->mode == UX_BUS_SIMULATION)
        return bus_simualtion_dequeue(ctx);
    else
        return bus_realtime_dequeue(ctx);
}

uxe_reminder_t* bus_timer_peek(ux_ctx_t *ctx, ux_clock_type type)
{
    assert(type < UX_CLOCK_LAST);
    return timer_heap_peek(&ctx->timer_heap[type]);
}

uxe_reminder_t* bus_timer_dequeue(ux_ctx_t *ctx, ux_clock_type type)
{
    assert(type < UX_CLOCK_LAST);
    return timer_heap_pop(&ctx->timer_heap[type]);
}

void bus_attach(ux_ctx_t* src, ux_ctx_t* dst)
{
    int num = src->attached_count++;
    src->attached[num] = ux_zalloc(sizeof(ux_queue_t));
    if (src->attached[num] != NULL) {
        ux_queue_init(src->attached[num], 10000, UX_CATEGORY_MARKET);
        bus_add_queue(dst, src->attached[num]);
    }
}

void bus_detach(ux_ctx_t* src, ux_ctx_t* dst)
{
    for (int i = 0; i < src->attached_count; i++) {
        ux_queue_t* q = src->attached[i];
        UX_ASSERT(q != NULL);
        if (q->ctx == dst) {
            bus_remove_queue(dst, q); // todo: add test(remove q from heap when q not insert heap)
            for (int j = i; j < src->attached_count - 1; j++)
                src->attached[j] = src->attached[j + 1];
            src->attached_count--;
        }
    }
}

void bus_add_queue(ux_ctx_t *ctx, ux_queue_t* q)
{
    UX_ASSERT(ctx != NULL);
    UX_ASSERT(q != NULL);

    UX_ASSERT(q->ctx == ctx || q->ctx == NULL);
    UX_ASSERT(q->category >= 0 && q->category < UX_CATEGORY_LAST);

    q->ctx = ctx;
    q->start_id = ctx->counter++;

    if (ux_queue_is_empty(q))
        return;

    heap_insert((struct heap*)&ctx->queue_heap[q->category],
        (struct heap_node*)&q->heap_node,
        queue_less_than);
}

void bus_remove_queue(ux_ctx_t *ctx, ux_queue_t* q)
{
    UX_ASSERT(ctx != NULL);
    UX_ASSERT(q != NULL);

    heap_remove((struct heap*)&ctx->queue_heap[q->category],
        (struct heap_node*)&q->heap_node,
        queue_less_than);
}

void bus_add_timer(ux_ctx_t *ctx, uxe_reminder_t* timer)
{
    UX_ASSERT(ctx != NULL);
    UX_ASSERT(timer != NULL);

    timer->ctx = ctx;
    timer->start_id = ctx->counter++;
    heap_insert((struct heap*)&ctx->timer_heap[timer->clock_type],
        (struct heap_node*)&timer->heap_node,
        timer_less_than);
}

void bus_remove_timer(ux_ctx_t *ctx, uxe_reminder_t* timer)
{
    UX_ASSERT(ctx != NULL);
    UX_ASSERT(timer != NULL);

    heap_remove((struct heap*)&ctx->timer_heap[timer->clock_type],
        (struct heap_node*)&timer->heap_node,
        timer_less_than);
}

ux_time_t bus_get_time(ux_ctx_t *ctx)
{
    UX_ASSERT(ctx != NULL);

    if (ctx->mode == UX_BUS_SIMULATION)
        return ctx->time[UX_CLOCK_LOCAL];
    else
        return ux_time_now();
}

int bus_set_time(ux_ctx_t *ctx, ux_time_t time)
{
    UX_ASSERT(ctx != NULL);

    if (ctx->mode == UX_BUS_REALTIME)
        return -1;

    if (time < ctx->time[UX_CLOCK_LOCAL]) {
        return -1;
    }
    ctx->time[UX_CLOCK_LOCAL] = time;
    return 0;
}

ux_time_t bus_get_exchange_time(ux_ctx_t *ctx)
{
    UX_ASSERT(ctx != NULL);

    return ctx->time[UX_CLOCK_EXCHANGE];
}

int bus_set_exchange_time(ux_ctx_t *ctx, ux_time_t time)
{
    UX_ASSERT(ctx != NULL);

    if (time < ctx->time[UX_CLOCK_EXCHANGE]) {
        return -1;
    }
    ctx->time[UX_CLOCK_EXCHANGE] = time;
    return 0;
}

void bus_init(ux_ctx_t *ctx)
{
    ctx->mode = UX_BUS_SIMULATION;

    mpscq_init(&ctx->pending_queue);
    for (int i = 0; i < UX_CATEGORY_LAST; i++)
        heap_init((struct heap*)&ctx->queue_heap[i]);

    heap_init((struct heap*)&ctx->timer_heap[UX_CLOCK_LOCAL]);
    heap_init((struct heap*)&ctx->timer_heap[UX_CLOCK_EXCHANGE]);

    ctx->time[UX_CLOCK_LOCAL] = MIN_DATE_TIME;
    ctx->time[UX_CLOCK_EXCHANGE] = MIN_DATE_TIME;

    ctx->counter = 1;
    ctx->saved_event = NULL;
    ctx->is_simulator_stop = 0;

    memset(&ctx->attached, '\0', sizeof(ctx->attached));
    ctx->attached_count = 0;

    ctx->event_count = 0;
}

void bus_destroy(ux_ctx_t *ctx)
{
    bus_clear(ctx);
}


void bus_clear(ux_ctx_t *ctx)
{
    if (ctx->saved_event != NULL) {
        ux_event_unref(ctx->saved_event);
        ctx->saved_event = NULL;
    }

    for (int i = 0; i < UX_CATEGORY_LAST; i++)
        queue_heap_clear(&ctx->queue_heap[i]);

    for (int i = 0; i < UX_CLOCK_LAST; i++)
        timer_heap_clear(&ctx->timer_heap[i]);

    for (int i = 0; i < ctx->attached_count; i++) {
        ux_queue_t* q = ctx->attached[i];
        bus_detach(ctx, q->ctx);
        ux_queue_destroy(q);
    }
    ctx->attached_count = 0;
}
