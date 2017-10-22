#ifndef __UXE_H__
#define __UXE_H__

#include <ux/base/ux_atomic.h>
#include <ux/base/ux_common.h>
#include <ux/base/ux_datetime.h>
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
UX_EXTERN ux_event_t *ux_event_malloc(uxe_type type);
UX_EXTERN ux_event_t *ux_event_ref(ux_event_t *e);
UX_EXTERN void ux_event_unref(ux_event_t *e);
UX_EXTERN ux_event_t *ux_event_clone(ux_event_t *e);

#ifdef __cplusplug
}
#endif

#endif // __UXE_H__
