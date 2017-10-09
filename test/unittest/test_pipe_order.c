#include "ux_internal.h"
#include "task.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 3
#define EVENT_SIZE 30

/* random [rmin, rmax] */
static int random_range(int rmin, int rmax)
{
    return (rand() % (rmax - rmin + 1)) + rmin;
}

static ux_time_t etime[EVENT_SIZE] = {
    8, 10, 34, 19, 11, 33, 25, 39, 4, 30,
    1, 6, 13, 3, 28, 24, 8, 10,
    1, 13, 24, 10, 30, 38, 1, 14, 20, 39, 17, 16

};

typedef struct {
    ux_time_t time;
    int qindex;
} result_except_t;

static result_except_t except[EVENT_SIZE] = {
    { 1, 1 }, { 1, 2 }, { 6, 1 }, { 8, 0 }, { 10, 0 }, { 13, 2 }, { 13, 1 }, { 3, 1 },
    { 24, 2 }, { 10, 2 }, { 28, 1 }, { 24, 1 }, { 8, 1 }, { 10, 1 },
    { 30, 2 }, { 34, 0 }, { 19, 0 }, { 11, 0 }, { 33, 0 }, { 25, 0 },
    { 38, 2 }, { 1, 2 }, { 14, 2 }, { 20, 2 }, { 39, 0 }, { 4, 0 }, { 30, 0 },
    { 39, 2 }, { 17, 2 }, { 16, 2 }
};

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

static int test_pipe_event_order(ux_bus_mode mode, ux_queue_category category)
{
    ux_loop_t loop;

    ux_queue_t queue[QUEUE_SIZE];

    ux_event_t* event[EVENT_SIZE];

    for (int i = 0; i < QUEUE_SIZE; i++) {
        ux_queue_init(&queue[i], 10000, category);
    }

    ux_loop_init(&loop);
    loop.mode = mode;

    /* prepare event */
    for (int i = 0; i < EVENT_SIZE; i++) {
        event[i] = ux_event_malloc(UX_EVENT_ASK);
        tick_init(event[i], i, i + 10000, 0, 1.0, i);
        event[i]->timestamp = etime[i];
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
        ((ux_event_tick_t*)event[i])->provider = 2;
    }

    bus_add_queue(&loop, &queue[0]);
    bus_add_queue(&loop, &queue[1]);
    bus_add_queue(&loop, &queue[2]);

    /* test event order */
    ux_event_t* e;
    int j = 0;

    while ((e = bus_dequeue(&loop)) != NULL) {
        ASSERT(e->timestamp == except[j].time);
#ifndef NDEBUG
        ASSERT_THEN(e->dummy == &queue[except[j].qindex],
                fprintf(stderr, "event index:%d time:%" PRIu64 " except [%d] but [%d]\n", j, e->timestamp, except[j].qindex, ((ux_event_tick_t*)e)->provider);
        );
#endif
        ux_event_unref(e);
        j++;
    }

    ASSERT(j == EVENT_SIZE);
    /* ASSERT event lifetime */
    for (int i = 0; i < EVENT_SIZE; i++) {
        ASSERT(event[i]->refcount == 0);
    }

    ASSERT(j == EVENT_SIZE);

    /* clear bus*/

    ux_loop_destory(&loop);
    return 0;
}

TEST_IMPL(bus_signle_pipe_event_order)
{
    // test diff mode && diff pipe

    for (int z = UX_BUS_SIMULATION; z < UX_BUS_MODE_LAST; z++) {
        for (int k = UX_CATEGORY_MARKET; k < UX_CATEGORY_LAST; k++) {
            test_pipe_event_order(z, k);
        }
    }
    return 0;
}
