/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_BUS_H__
#define __UX_BUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ux/event/tick.h>

typedef struct {
  void *min;
  unsigned int nelts;
} min_heap;

typedef enum {
    UX_BUS_SIMULATION,
    UX_BUS_REALTIME,
    UX_BUS_MODE_LAST
} ux_bus_mode;


#define EVENT_BUS_FIELDS                                                       \
  ux_bus_mode mode;                                                            \
  mpscq_t pending_queue;                                                       \
  spscq_t buffed_event_queue;                                                  \
  min_heap queue_heap[UX_CATEGORY_LAST];                                       \
  min_heap timer_heap[UX_CLOCK_LAST];                                          \
  uint64_t counter;                                                            \
  ux_time_t time[UX_CLOCK_LAST];                                               \
  ux_event_t *saved_event;                                                     \
  ux_queue_t *attached[128];                                                   \
  int attached_count;                                                          \
  int is_simulator_stop;


#ifdef __cplusplus
}
#endif

#endif // __UX_BUS_H__
