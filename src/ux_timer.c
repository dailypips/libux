/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_timer.h"

#include "heap-inl.h"

#include <assert.h>
#include <limits.h>

int atl_timer_init(atl_loop_t* loop, atl_timer_t* handle) {
  atl__handle_init(loop, (atl_handle_t*)handle, UV_TIMER);
  handle->timer_cb = NULL;
  handle->repeat = 0;
  return 0;
}


int atl_timer_start(atl_timer_t* handle,
                   atl_timer_cb cb,
                   uint64_t timeout,
                   uint64_t repeat) {
  uint64_t clamped_timeout;

  if (cb == NULL)
    return -EINVAL;

  if (atl__is_active(handle))
    atl_timer_stop(handle);

  clamped_timeout = handle->loop->time + timeout;
  if (clamped_timeout < timeout)
    clamped_timeout = (uint64_t) -1;

  handle->timer_cb = cb;
  handle->timeout = clamped_timeout;
  handle->repeat = repeat;
  /* start_id is the second index to be compared in atl__timer_cmp() */
  handle->start_id = handle->loop->timer_counter++;

  heap_insert((struct heap*) &handle->loop->timer_heap,
              (struct heap_node*) &handle->heap_node,
              timer_less_than);
  atl__handle_start(handle);

  return 0;
}


int atl_timer_stop(atl_timer_t* handle) {
  if (!atl__is_active(handle))
    return 0;

  heap_remove((struct heap*) &handle->loop->timer_heap,
              (struct heap_node*) &handle->heap_node,
              timer_less_than);
  atl__handle_stop(handle);

  return 0;
}


int atl_timer_again(atl_timer_t* handle) {
  if (handle->timer_cb == NULL)
    return -EINVAL;

  if (handle->repeat) {
    atl_timer_stop(handle);
    atl_timer_start(handle, handle->timer_cb, handle->repeat, handle->repeat);
  }

  return 0;
}


void atl_timer_set_repeat(atl_timer_t* handle, uint64_t repeat) {
  handle->repeat = repeat;
}


uint64_t atl_timer_get_repeat(const atl_timer_t* handle) {
  return handle->repeat;
}


int atl__next_timeout(const atl_loop_t* loop) {
  const struct heap_node* heap_node;
  const atl_timer_t* handle;
  uint64_t diff;

  heap_node = heap_min((const struct heap*) &loop->timer_heap);
  if (heap_node == NULL)
    return -1; /* block indefinitely */

  handle = container_of(heap_node, atl_timer_t, heap_node);
  if (handle->timeout <= loop->time)
    return 0;

  diff = handle->timeout - loop->time;
  if (diff > INT_MAX)
    diff = INT_MAX;

  return diff;
}

static void atl__fire_timer(atl_timer_t *handle)
{
    atl_timer_stop(handle);
    atl_timer_again(handle);
    handle->timer_cb(handle);
}

void atl__run_timers(atl_loop_t* loop) {
  struct heap_node* heap_node;
  atl_timer_t* handle;

  for (;;) {
    heap_node = heap_min((struct heap*) &loop->timer_heap);
    if (heap_node == NULL)
      break;

    handle = container_of(heap_node, atl_timer_t, heap_node);
    if (handle->timeout > loop->time)
      break;

    atl__fire_timer(handle);
  }
}


void atl__timer_close(atl_timer_t* handle) {
  atl_timer_stop(handle);
}
