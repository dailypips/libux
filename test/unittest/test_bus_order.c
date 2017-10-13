#include "ux_internal.h"
#include "task.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 3
#define EVENT_SIZE 30
#define TIMER_SIZE 5

static ux_time_t etime[EVENT_SIZE] = {
    8, 10, 34, 19, 11, 33, 25, 39, 4, 30,
    1, 6, 13, 3, 28, 24, 8, 10,
    1, 13, 24, 10, 30, 38, 1, 14, 20, 39, 17, 16

};

static ux_time_t timer_time[TIMER_SIZE] = {
    15, 30, 25, 9, 11
};

typedef struct {
    ux_time_t time;
    int qindex;
} result_except_t;

static result_except_t except[EVENT_SIZE] = {
    {1, 2}, {13, 2}, {24, 2}, {10, 2}, {30, 2}, {38, 2}, {1, 2}, {14, 2}, {20, 2}, {39, 2}, {17, 2}, {16, 2},
    {1, 1}, {6, 1}, {8, 0}, {10, 0}, {13, 1}, {3, 1}, {28, 1}, {24, 1}, {8, 1}, {10, 1},
    {34, 0}, {19, 0}, {11, 0}, {33, 0}, {25, 0}, {39, 0}, {4, 0}, {30, 0}
};

static ux_time_t except_timer[TIMER_SIZE] = {
    9, 11, 15, 25, 30
};

static ux_time_t all_time[EVENT_SIZE + TIMER_SIZE] = {
    1, 13, 24, 10, 30, 38, 1, 14, 20, 39, 17, 16, // first pop all execution message
    1, 6, 8, 9, // last is timer
    10, 11,     // last is timer
    13, 3, 15, 25, // last and before is timer
    28, 24, 8, 10, 30, // last is timer
    34, 19, 11, 33, 25, 39, 4, 30
};
static void on_reminder(ux_event_reminder_t *r)
{
    UX_UNUSED(r);
}

static ux_event_t* tick_init(ux_event_t* e, int provider, int instrument, ux_time_t exchange_timestamp, double price, long size)
{
    ux_event_tick_t* tick = (ux_event_tick_t*)e;
    tick->exchange_timestamp = exchange_timestamp;
    tick->instrument = instrument;
    tick->provider = provider;
    tick->price = price;
    tick->size = size;
    return e;
}

static void timer_init(ux_event_reminder_t* e, ux_clock_type ctype, ux_time_t timeout)
{
    e->clock_type = ctype;
    e->callback = on_reminder;
    e->user_data = e;
    e->timestamp = timeout;
    e->timeout = timeout;
    e->repeat = 0;
}

static int test_event_order(ux_bus_mode mode)
{
    ux_ctx_t ctx;

    ux_queue_t queue[QUEUE_SIZE];

    ux_event_t* event[EVENT_SIZE];
    ux_event_reminder_t* timer[TIMER_SIZE];

    for (int i = 0; i < QUEUE_SIZE; i++) {
        ux_queue_init(&queue[i], 10000, UX_CATEGORY_MARKET);
    }

    ux_ctx_init(&ctx);
    ctx.mode = mode;

    /* prepare event */
    for (int i = 0; i < EVENT_SIZE; i++) {
        event[i] = ux_event_malloc(UX_EVENT_ASK);
        tick_init(event[i], i, i + 10000, 0, 1.0, i);
        event[i]->timestamp = etime[i];
    }

    for (int i = 0; i < TIMER_SIZE; i++) {
        timer[i] = (ux_event_reminder_t*)ux_event_malloc(UX_EVENT_REMINDER);
        timer_init(timer[i], UX_CLOCK_LOCAL, timer_time[i]);
        bus_add_timer(&ctx, timer[i]);
    }

    for (int i = 0; i < 10; i++) {
        ux_queue_push(&queue[0], event[i]);
        ((ux_event_tick_t*)event[i])->provider = 0;
    }

    for (int i = 10; i < 18; i++) {
        ux_queue_push(&queue[1], event[i]);
        ((ux_event_tick_t*)event[i])->provider = 1;
    }

    for (int i = 18; i < 30; i++) {
        ux_queue_push(&queue[2], event[i]);
        queue[2].category = UX_CATEGORY_EXECUTION;
        ((ux_event_tick_t*)event[i])->provider = 2;
    }

    bus_add_queue(&ctx, &queue[0]);
    bus_add_queue(&ctx, &queue[1]);
    bus_add_queue(&ctx, &queue[2]);

    /* test event order */
    ux_event_t* e;

    int k = 0;
    int z= 0;
    while ((e = bus_dequeue(&ctx)) != NULL) {
        ASSERT(e->timestamp == all_time[z]);
        if (e->type == UX_EVENT_ASK)
        {
#ifndef NDEBUG
            ASSERT_THEN(e->dummy == &queue[except[k].qindex],
                    fprintf(stderr, "event index:%d time:%" PRIu64 " except [%d] but [%d]\n", k, e->timestamp, except[k].qindex, ((ux_event_tick_t*)e)->provider);
            );
#endif
            k++;
        }
        ux_event_unref(e);
        z++;
    }
    ASSERT(z == TIMER_SIZE + EVENT_SIZE);
    ASSERT(k == EVENT_SIZE);
    /* ASSERT event lifetime */
    for (int i = 0; i < EVENT_SIZE; i++) {
        ASSERT(event[i]->refcount == 0);
    }

    for (int i = 0; i < TIMER_SIZE; i++) {
        ASSERT(timer[i]->refcount == 0);
    }

    /* clear bus*/

    ux_ctx_destory(&ctx);
    return 0;
}

TEST_IMPL(bus_full_event_order)
{
    test_event_order(UX_BUS_SIMULATION);

    return 0;
}
