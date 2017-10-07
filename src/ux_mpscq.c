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

#include "ux_mpscq.h"

void ux_mpscq_init(ux_mpscq_t *q) {
  ux_atomic_no_barrier_store(&q->head, (ux_atomic_t)&q->sentinel);
  q->tail = &q->sentinel;
  ux_atomic_no_barrier_store(&q->sentinel.next, (ux_atomic_t)NULL);
}

void ux_mpscq_destroy(ux_mpscq_t *q) {
  UX_ASSERT(ux_atomic_no_barrier_load(&q->head) == (ux_atomic_t)&q->sentinel);
  UX_ASSERT(q->tail == &q->sentinel);
}

void ux_mpscq_push(ux_mpscq_t *q, ux_mpscq_node *n) {
  ux_atomic_no_barrier_store(&n->next, (ux_atomic_t)NULL);
  ux_mpscq_node *prev =
      (ux_mpscq_node *)ux_atomic_full_xchg(&q->head, (ux_atomic_t)n);
  ux_atomic_rel_store(&prev->next, (ux_atomic_t)n);
}

ux_mpscq_node *ux_mpscq_pop(ux_mpscq_t *q) {
  bool empty;
  return ux_mpscq_pop_and_check_end(q, &empty);
}

ux_mpscq_node *ux_mpscq_pop_and_check_end(ux_mpscq_t *q, bool *empty) {
  ux_mpscq_node *tail = q->tail;
  ux_mpscq_node *next = (ux_mpscq_node *)ux_atomic_acq_load(&tail->next);
  if (tail == &q->sentinel) {
    // indicates the list is actually (ephemerally) empty
    if (next == NULL) {
      *empty = true;
      return NULL;
    }
    q->tail = next;
    tail = next;
    next = (ux_mpscq_node *)ux_atomic_acq_load(&tail->next);
  }
  if (next != NULL) {
    *empty = false;
    q->tail = next;
    return tail;
  }
  ux_mpscq_node *head = (ux_mpscq_node *)ux_atomic_acq_load(&q->head);
  if (tail != head) {
    *empty = false;
    // indicates a retry is in order: we're still adding
    return NULL;
  }
  ux_mpscq_push(q, &q->sentinel);
  next = (ux_mpscq_node *)ux_atomic_acq_load(&tail->next);
  if (next != NULL) {
    q->tail = next;
    return tail;
  }
  // indicates a retry is in order: we're still adding
  *empty = false;
  return NULL;
}
