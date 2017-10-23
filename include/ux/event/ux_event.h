/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_EVENT_H__
#define __UX_EVENT_H__

#include <ux/base/ux_type.h>
#include <ux/event/event_def.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
#define EVENTENUM(name, lname, destroy, clone, processor) UXE_##name,
  EVENTDEF(EVENTENUM)
#undef EVENTENUM
      UXE_DEF_LAST,
  UXE_CUSTOM = 512,
  UXE_LAST = 1024
} uxe_type;

#define EVENTETYPE(name, lname, destroy, clone, processor) typedef struct uxe_##lname##_s uxe_##lname##_t;
EVENTDEF(EVENTETYPE)
#undef EVENTETYPE

#ifdef NDEBUG
#define UXE_COMMON_FIELDS                                                      \
  ux_atomic_t refcount;                                                        \
  uxe_type type;                                                               \
  ux_time_t timestamp;
#else
#define UXE_COMMON_FIELDS                                                      \
  ux_atomic_t refcount;                                                        \
  uxe_type type;                                                               \
  ux_time_t timestamp;                                                         \
  void *dummy;
#endif

typedef struct ux_event_s {
  UXE_COMMON_FIELDS
} ux_event_t;

UX_EXTERN size_t ux_event_size(uxe_type type);
UX_EXTERN ux_event_t *ux_event_zalloc(uxe_type type);
UX_EXTERN ux_event_t *ux_event_ref(ux_event_t *e);
UX_EXTERN void ux_event_unref(ux_event_t *e);
UX_EXTERN ux_event_t *ux_event_clone(ux_event_t *e);

#define EVENT_NEW(x) (void*)ux_event_zalloc((x))

#ifdef __cplusplug
}
#endif

#endif // __UX_EVENT_H__
