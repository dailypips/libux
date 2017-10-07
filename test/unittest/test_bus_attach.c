#include "../src/ux_internal.h"
#include "task.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 3
#define EVENT_SIZE QUEUE_SIZE * 10

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

static int test_attach(ux_bus_mode mode)
{
    ux_loop_t loop1, loop2;

    ux_queue_t queue[QUEUE_SIZE];
    ux_event_t* event[EVENT_SIZE];

    for (int i = 0; i < QUEUE_SIZE; i++) {
        ux_queue_init(&queue[i], 10000, UX_CATEGORY_MARKET);
    }

    ux_loop_init(&loop1);
    ux_loop_init(&loop2);
    loop1.mode = mode;
    loop2.mode = mode;

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

    bus_add_queue(&loop1, &queue[0]);
    bus_add_queue(&loop1, &queue[1]);
    bus_add_queue(&loop1, &queue[2]);


    bus_attach(&loop1, &loop2);

    /* test event order */
    ux_event_t* e1, *e2;
    int j = 0;

    while ((e1 = bus_next_event(&loop1)) != NULL) {
        e2 = bus_next_event(&loop2);
        ASSERT_THEN(e1 == e2,
            fprintf(stderr, "index:%d e1: %p, e2:%p\n", j, e1, e2);
       );
        ux_event_unref(e1);
        ux_event_unref(e1);
        j++;
    }

    ASSERT(j == EVENT_SIZE);
    /* ASSERT event lifetime */
    for (int i = 0; i < EVENT_SIZE; i++) {
        ASSERT(event[i]->refcount == 0);
    }

    /* clear bus*/
    bus_detach(&loop1, &loop2);
    ux_loop_destory(&loop1);
    ux_loop_destory(&loop2);
    return 0;
}

TEST_IMPL(bus_attach)
{
    test_attach(UX_BUS_SIMULATION);
    return 0;
}
