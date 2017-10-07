/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

/* public api */
size_t ux_event_size(ux_event_type type)
{
    return g_eventclassinfo[type].size;
}

static void event_free(ux_event_t *e)
{
    event_destory destory = g_eventclassinfo[e->type].destory;
    if (destory)
        destory(e);
    ux_free(e);
}

ux_event_t *ux_event_clone(ux_event_t *e)
{
    ux_event_t *result;
    event_clone clone = g_eventclassinfo[e->type].clone;
    if(clone)
        result = clone(e);

    return result;
}

ux_event_t *ux_event_ref(ux_event_t *event)
{
  assert(event != NULL);
  ux_atomic_full_fetch_add(&event->refcount, 1);
  return event;
}

void ux_event_unref(ux_event_t *event)
{
   assert(event != NULL);
   if(ux_atomic_full_fetch_add(&event->refcount, -1) == 1)
       event_free(event);
}

ux_event_t* ux_event_malloc(ux_event_type type)
{
    assert(type < UX_EVENT_LAST);
    ux_event_t* e = ux_zalloc(g_eventclassinfo[type].size);
    if (e) {
        e->type = type;
        ux_atomic_rel_store(&e->refcount, 1);
    }
    return e;
}
