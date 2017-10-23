/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __EVENT_VTABLE_H__
#define __EVENT_VTABLE_H__

#include <ux/bus/ux_context.h>
#include <ux/event/ux_event.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*event_destroy)(ux_event_t *e);
typedef ux_event_t *(*event_clone)(ux_event_t *e);
typedef void (*event_dispatch)(ux_ctx_t *ctx, ux_event_t *e);

typedef struct _uxe_class_vtable_t {
  size_t size;
  event_destroy destroy;
  event_clone clone;
  event_dispatch dispatch;
} uxe_class_vtable_t;

extern uxe_class_vtable_t g_event_vtable[UXE_LAST];


#define CLASSENUM(euname, elname, edestroy, eclone, edispatch)   \
    void event_##edestroy##_destroy(ux_event_t *e);\
     ux_event_t* event_##eclone##_clone(ux_event_t *e);\
     void event_##edispatch##_dispatch(ux_ctx_t *ctx, ux_event_t *e);
    EVENTDEF(CLASSENUM)
#undef CLASSENUM


#ifdef __cplusplus
}
#endif

#endif // __EVENT_VTABLE_H__
