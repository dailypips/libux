/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef UX_BUS_H
#define UX_BUS_H

#include <stdint.h>
#include "ux_common.h"
#include "ux_event.h"
#include "ux_queue.h"

typedef struct {
    void *min;
    unsigned int nelts;
}min_heap;

typedef enum {
    UX_BUS_SIMULATION,
    UX_BUS_REALTIME
}ux_bus_mode;

typedef struct ux_bus_s {
    ux_bus_mode mode;
    min_heap queue_heap[UX_CATEGORY_LAST];
    min_heap timer_heap[UX_CLOCK_LAST];
    uint64_t counter;
    datetime_t time[UX_CLOCK_LAST];
    ux_event_t *saved_event;
    ux_queue_t attached[128];
    int attached_count;
}ux_bus_t;

void ux_bus_init(ux_bus_t *bus, ux_bus_mode mode);
void ux_bus_destory(ux_bus_t *bus);

void ux_bus_add_queue(ux_bus_t *bus, void *q);
void ux_bus_remove_queue(ux_bus_t *bus, void *q);

void ux_bus_add_timer(ux_bus_t *bus, ux_event_reminder_t* timer);
void ux_bus_remove_timer(ux_bus_t *bus, ux_event_reminder_t* timer);

ux_event_t* ux_bus_next_event(ux_bus_t *bus);
int64_t ux_bus_next_timeout(ux_bus_t* bus);

#endif // UX_BUS_H
