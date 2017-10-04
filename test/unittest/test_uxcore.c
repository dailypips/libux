/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <uv.h>
#include "task.h"
#include "../src/ux_bus.h"
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 3
#define EVENT_SIZE QUEUE_SIZE * 10

/* random [rmin, rmax] */
int random_range(int rmin, int rmax)
{
    return (rand() % (rmax-rmin+1))+ rmin;
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

ux_event_t * event_tick_init(ux_event_t *e, int provider, int instrument, datetime_t exchange_timestamp, double price, long size)
{
    ux_event_tick_t * tick = (ux_event_tick_t *)e;
    tick->exchange_timestamp = exchange_timestamp;
    tick->instrument = instrument;
    tick->provider = provider;
    tick->price = price;
    tick->size = size;
    return e;
}

TEST_IMPL(ux_core_test) {

  ux_bus_t bus;

  ux_queue_t queue[QUEUE_SIZE];

  ux_event_t* event[EVENT_SIZE];

  for (int i = 0 ; i < QUEUE_SIZE; i++) {
      ux_queue_init(&queue[i],10000, UX_CATEGORY_MARKET);
  }

  ux_bus_init(&bus, UX_BUS_SIMULATION);
  printf("h1");
  /* prepare event */
  for (int i = 0; i < EVENT_SIZE; i++) {
      event[i] = ux_event_malloc(UX_EVENT_ASK);
      event_tick_init(event[i], i, i + 10000, 0, 1.0, i);
      event[i]->timestamp = etime[i];
  }
  printf("h2");

  for (int i = 0; i < 10; i ++)
      ux_queue_push(&queue[0], event[i]);

  for(int i = 10; i < 18; i++)
      ux_queue_push(&queue[1], event[i]);

  for(int i = 18; i < 30; i++)
      ux_queue_push(&queue[2], event[i]);

    printf("h13");
  ux_bus_add_queue(&bus, &queue[0]);
  ux_bus_add_queue(&bus, &queue[1]);
  ux_bus_add_queue(&bus, &queue[2]);
  printf("h4");
    ux_event_t *e;
    int j = 0;
  while((e= ux_bus_next_event(&bus)) != NULL) {
      assert(e->timestamp == expect_time[j]);
      j++;
  }
  printf("h5");
  //ux_run(loop, UX_RUN_DEFAULT);

  /* clean event */
  for(int i = 0; i < EVENT_SIZE; i++) {
      ux_event_unref(event[i]);
  }

  /* clear queue*/

  return 0;
}
