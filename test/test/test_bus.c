
#include "../src/ux_bus.h"
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#define QUEUE_SIZE 3
#define EVENT_SIZE 30

/* random [rmin, rmax] */
int random_range(int rmin, int rmax)
{
    return (rand() % (rmax - rmin + 1)) + rmin;
}

datetime_t etime[EVENT_SIZE] = {
    8, 10, 34, 19, 11, 33, 25, 39, 4, 30,
    1, 6, 13, 3, 28, 24, 8, 10,
    1, 13, 24, 10, 30, 38, 1, 14, 20, 39, 17, 16

};

datetime_t expect_time[EVENT_SIZE] = {
    8, 10, 34, 19, 11, 33, 25, 39, 4, 30,
    1, 6, 13, 3, 28, 24, 8, 10,
    1, 13, 24, 10, 30, 38, 1, 14, 20, 39, 17, 16
};

int main(void)
{
    ux_bus_t bus;
    ux_queue_t queue[QUEUE_SIZE];
    ux_event_t* events[EVENT_SIZE];

    ux_bus_init(&bus, UX_BUS_SIMULATION);

    for (int i = 0; i < QUEUE_SIZE; i++) {
        ux_queue_init(&queue[i], 10000, UX_CATEGORY_MARKET);
    }

    /* prepare event */
    for (int i = 0; i < EVENT_SIZE; i++) {
        //events[i] = ux_event_new(UX_EVENT_ASK, /*instrument*/ i, /*provider*/ i, /*exchange_timestamp*/ 0, /*price*/ 1.0, /*size*/ i);
        events[i] = ux_event_malloc(UX_EVENT_ASK);
        ux_event_ask_t *ask = (ux_event_ask_t *)(events[i]);
        ask->instrument = i;
        ask->provider = i;
        ask->exchange_timestamp = 0;
        ask->price = 1.0;
        ask->size = i;
        events[i]->timestamp = etime[i];
    }

    printf("AllEvent:\n");
    for (int i = 0; i < EVENT_SIZE; i++) {
        ux_event_t *e = events[i];
        ux_event_tick_t* tick = (ux_event_tick_t*)e;
        printf("time = %llu index = %ld\n", e->timestamp, tick->size);
    }
    printf("End\n");
    for (int i = 0; i < 10; i++) {
        ux_queue_push(&queue[0], events[i]);
        ux_event_t *e = events[i];
        ux_event_tick_t* tick = (ux_event_tick_t*)e;
        tick->provider = 0;
    }

    for (int i = 10; i < 18; i++) {
        ux_queue_push(&queue[1], events[i]);
        ux_event_t *e = events[i];
        ux_event_tick_t* tick = (ux_event_tick_t*)e;
        tick->provider = 1;
    }

    for (int i = 18; i < 30; i++) {
        ux_queue_push(&queue[2], events[i]);
        ux_event_t *e = events[i];
        ux_event_tick_t* tick = (ux_event_tick_t*)e;
        tick->provider = 2;
    }

#if 0
  unsigned int size0 = ux_queue_size(&queue[0]);
  unsigned int size1 = ux_queue_size(&queue[1]);
  unsigned int size2 = ux_queue_size(&queue[2]);
  printf("queue size %u %u %u\n", size0, size1, size2);
#endif

    ux_bus_add_queue(&bus, &queue[0]);
    ux_bus_add_queue(&bus, &queue[1]);
    ux_bus_add_queue(&bus, &queue[2]);


    ux_event_t* e;
    int j = 0;
    while ((e = ux_bus_next_event(&bus)) != NULL) {
        //assert(e->timestamp == expect_time[j]);
        ux_event_tick_t* tick = (ux_event_tick_t*)e;
        printf("time = %llu qindex = %d eindex=%ld\n", e->timestamp, tick->provider, tick->size);
        j++;
    }

    printf("event count = %d\n", j);
    //ux_run(loop, UX_RUN_DEFAULT);

    /* clean event */
#if 0
    for (int i = 0; i < EVENT_SIZE; i++) {
        ux_event_unref(event[i]);
    }
#endif

    /* clear queue*/

    return 0;
}
