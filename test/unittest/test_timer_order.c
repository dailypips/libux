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

static ux_time_t except[EVENT_SIZE] = {
    1,1,1,3,4,6,8,8,10,10,10,11,13,13,14,16,17,19,20,24,24,25,28,30,30,33,34,38,39,39
};

static void on_reminder(uxe_reminder_t *r)
{
    UX_UNUSED(r);
}

static void timer_init(uxe_reminder_t* e, ux_clock_type ctype, ux_time_t timeout)
{
    e->clock_type = ctype;
    e->callback = on_reminder;
    e->user_data = e;
    e->timestamp = timeout;
    e->timeout = timeout;
    e->repeat = 0;
}

static int test_timer_event_order(ux_bus_mode mode, ux_clock_type ctype)
{
    fprintf(stderr, "mode = %d clock_type = %d\n", mode, ctype);
    ux_ctx_t ctx;

    uxe_reminder_t* event[EVENT_SIZE];

    ux_ctx_init(&ctx);
    ctx.mode = mode;

    /* prepare event */
    for (int i = 0; i < EVENT_SIZE; i++) {
        event[i] = (uxe_reminder_t*)ux_event_malloc(UXE_REMINDER);
        timer_init(event[i], ctype, etime[i]);
        bus_add_timer(&ctx, event[i]);
    }

    /* test timer order */
    int j = 0;

    uxe_reminder_t *e;

    while ((e = bus_timer_dequeue(&ctx, ctype)) != NULL) {

        ASSERT_THEN(e->timestamp == except[j],
                    fprintf(stderr, "[%d] except %"PRIu64" but %"PRIu64"\n", j, except[j], e->timestamp);
                );
        ux_event_unref((ux_event_t*)e);
        j++;
    }

    ASSERT(j == EVENT_SIZE);
    /* ASSERT event lifetime */
    for (int i = 0; i < EVENT_SIZE; i++) {
        ASSERT(event[i]->refcount == 0);
    }

    /* clear bus*/

    ux_ctx_destroy(&ctx);

    return 0;
}

TEST_IMPL(bus_signle_timer_event_order)
{
    // test diff mode && diff pipe

    for (int z = UX_BUS_SIMULATION; z < UX_BUS_MODE_LAST; z++) {
        for (int k = UX_CLOCK_LOCAL; k < UX_CLOCK_LAST; k++) {
            test_timer_event_order(z, k);
        }
    }
    return 0;
}
