/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

#include <ux/bus/ux_queue.h>
#include "mpscq.h"
#include "spscq.h"
#include <ux/base/ux_memory.h>
#include <ux/event/ux_event.h>

struct ux_queue_s {
  void *data;
  /* private */
  ux_atomic_t refcount;
  spscq_t spsc;
  /* only access by consumer */
  void *ctx;
  ux_queue_category category;
  /* mpsc */
  mpscq_node mpsc_node;
  /* min heap node */
  void *heap_node[3];
  uint64_t start_id;
};

UX_FUNC ux_queue_t *ux_queue_init(ux_queue_t *q, unsigned int size,
                                  ux_queue_category category);
UX_FUNC void ux_queue_destroy(ux_queue_t *q);

UX_FUNC void ux_queue_ref(ux_queue_t *q);
UX_FUNC void ux_queue_unref(ux_queue_t *q);

UX_FUNC unsigned int ux_queue_size(ux_queue_t *q);
UX_FUNC unsigned int ux_queue_capacity(ux_queue_t *q);

UX_FUNC void *ux_queue_peek(const ux_queue_t *q);
UX_FUNC void *ux_queue_pop(ux_queue_t *q);

UX_FUNC int ux_queue_is_empty(ux_queue_t *q);
UX_FUNC int ux_queue_is_full(ux_queue_t *q);

#endif // __EVENT_QUEUE_H__
