/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"
#include <ux/datetime.h>
#include <ux/memory.h>
#include <ux/event.h>


extern uxe_class_vtable_t g_event_vtable[UXE_LAST];

/* check event is ux defined */
static int check_event_type(uxe_type type)
{
    if (type >= UXE_DEF_LAST)
        return 1;
    else
        return 0;
}

/* public api */
size_t ux_event_size(uxe_type type)
{
    return g_event_vtable[type].size;
}

static void event_free(ux_event_t *e)
{
    event_destory destory = g_event_vtable[e->type].destory;
    if (destory)
        destory(e);
    ux_free(e);
}

ux_event_t *ux_event_clone(ux_event_t *e)
{
    ux_event_t *result;
    event_clone clone = g_event_vtable[e->type].clone;
    if(clone)
        result = clone(e);

    return result;
}

ux_event_t *ux_event_ref(ux_event_t *event)
{
  UX_ASSERT(event != NULL);
  ux_atomic_full_fetch_add(&event->refcount, 1);
  return event;
}

void ux_event_unref(ux_event_t *event)
{
   UX_ASSERT(event != NULL);
   if(ux_atomic_full_fetch_add(&event->refcount, -1) == 1)
       event_free(event);
}

ux_event_t* ux_event_malloc(uxe_type type)
{
    UX_ASSERT(type < UXE_LAST);
    ux_event_t* e = ux_zalloc(g_event_vtable[type].size);
    if (e) {
        e->type = type;
        ux_atomic_rel_store(&e->refcount, 1);
    }
    return e;
}
