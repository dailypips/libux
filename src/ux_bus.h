/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_BUS_H__
#define __UX_BUS_H__

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
    UX_BUS_REALTIME,
    UX_BUS_MODE_LAST
}ux_bus_mode;

typedef struct ux_bus_s {
    ux_bus_mode mode;
    min_heap queue_heap[UX_CATEGORY_LAST];
    min_heap timer_heap[UX_CLOCK_LAST];
    uint64_t counter;
    datetime_t time[UX_CLOCK_LAST];
    ux_event_t *saved_event;
    ux_queue_t* attached[128];
    int attached_count;
    int is_simulator_stop;
}ux_bus_t;

UX_FUNC void ux_bus_init(ux_bus_t *bus, ux_bus_mode mode);
UX_FUNC void ux_bus_destory(ux_bus_t *bus);

UX_FUNC void ux_bus_clear(ux_bus_t *bus);

UX_FUNC void ux_bus_attach(ux_bus_t* src, ux_bus_t *dst);
UX_FUNC void ux_bus_detach(ux_bus_t* src, ux_bus_t *dst);

UX_FUNC void ux_bus_add_queue(ux_bus_t *bus, void *q);
UX_FUNC void ux_bus_remove_queue(ux_bus_t *bus, void *q);

UX_FUNC void ux_bus_add_timer(ux_bus_t *bus, ux_event_reminder_t* timer);
UX_FUNC void ux_bus_remove_timer(ux_bus_t *bus, ux_event_reminder_t* timer);

UX_FUNC ux_event_t* ux_bus_next_event(ux_bus_t *bus);
UX_FUNC int64_t ux_bus_next_timeout(ux_bus_t* bus);
UX_FUNC ux_event_reminder_t* ux_bus_next_reminder(ux_bus_t* bus, ux_clock_type type);

UX_FUNC datetime_t ux_bus_get_time(ux_bus_t* bus);
UX_FUNC int ux_bus_set_time(ux_bus_t* bus, datetime_t time);

UX_FUNC datetime_t ux_bus_get_exchange_time(ux_bus_t* bus);
UX_FUNC int ux_bus_set_exchange_time(ux_bus_t* bus, datetime_t time);

#endif // __UX_BUS_H__
