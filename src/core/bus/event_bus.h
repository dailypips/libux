/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __EVENT_BUS_H__
#define __EVENT_BUS_H__

#include <ux/base/ux_memory.h>
#include <ux/bus/ux_context.h>
#include <ux/event/ux_event.h>
#include <ux/bus/ux_bus.h>
#include "mpscq.h"
#include "spscq.h"
#include <ux/bus/ux_queue.h>
#include <ux/event/tick.h>

#ifdef __cplusplus
extern "C" {
#endif

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


UX_FUNC void bus_init(ux_ctx_t *ctx);
UX_FUNC void bus_destroy(ux_ctx_t *ctx);
UX_FUNC void bus_clear(ux_ctx_t *ctx);

UX_FUNC void bus_attach(ux_ctx_t *src, ux_ctx_t *dst);
UX_FUNC void bus_detach(ux_ctx_t *src, ux_ctx_t *dst);

UX_FUNC void bus_add_queue(ux_ctx_t *ctx, ux_queue_t *q);
UX_FUNC void bus_remove_queue(ux_ctx_t *ctx, ux_queue_t *q);

UX_FUNC void bus_add_timer(ux_ctx_t *ctx, uxe_reminder_t *timer);
UX_FUNC void bus_remove_timer(ux_ctx_t *ctx, uxe_reminder_t *timer);

UX_FUNC ux_event_t *bus_dequeue(ux_ctx_t *ctx);
UX_FUNC uxe_reminder_t *bus_timer_peek(ux_ctx_t *ctx, ux_clock_type type);
UX_FUNC uxe_reminder_t *bus_timer_dequeue(ux_ctx_t *ctx, ux_clock_type type);

UX_FUNC ux_time_t bus_get_time(ux_ctx_t *ctx);
UX_FUNC int bus_set_time(ux_ctx_t *ctx, ux_time_t time);

UX_FUNC ux_time_t bus_get_exchange_time(ux_ctx_t *ctx);
UX_FUNC int bus_set_exchange_time(ux_ctx_t *ctx, ux_time_t time);

#ifdef __cplusplus
}
#endif

#endif // __EVENT_BUS_H__
