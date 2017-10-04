/*
 *
 * Copyright 2016 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __UX_MPSCQ_H__
#define __UX_MPSCQ_H__

#include <stdbool.h>
#include <stddef.h>

#include "ux_atomic.h"

// Multiple-producer single-consumer lock free queue, based upon the
// implementation from Dmitry Vyukov here:
// http://www.1024cores.net/home/lock-free-algorithms/queues/intrusive-mpsc-node-based-queue

// List node (include this in a data structure at the top, and add application
// fields after it - to simulate inheritance)
typedef struct ux_mpscq_node { ux_atomic_t next; } ux_mpscq_node;

// Actual queue type
typedef struct ux_mpscq_t {
  ux_atomic_t head;
  // make sure head & tail don't share a cacheline
  char padding[UX_CACHELINE_SIZE];
  ux_mpscq_node *tail;
  ux_mpscq_node stub;
} ux_mpscq_t;

void ux_mpscq_init(ux_mpscq_t *q);
void ux_mpscq_destroy(ux_mpscq_t *q);
// Push a node
void ux_mpscq_push(ux_mpscq_t *q, ux_mpscq_node *n);
// Pop a node (returns NULL if no node is ready - which doesn't indicate that
// the queue is empty!!)
ux_mpscq_node *ux_mpscq_pop(ux_mpscq_t *q);
// Pop a node; sets *empty to true if the queue is empty, or false if it is not
ux_mpscq_node *ux_mpscq_pop_and_check_end(ux_mpscq_t *q, bool *empty);

#endif /* __UX_MPSCQ_H__ */
