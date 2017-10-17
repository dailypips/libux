#ifndef __UXE_H__
#define __UXE_H__

#include <ux/common.h>
#include <ux/types.h>
#include <ux/datetime.h>
#include <ux/event_def.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
#define EVENTENUM(name, lname, destory, clone, processor)	UXE_##name,
EVENTDEF(EVENTENUM)
#undef EVENTENUM
    UXE_LAST
} uxe_type;

#ifdef NDEBUG
#define UXE_COMMON_FIELDS \
    ux_atomic_t refcount;        \
    uxe_type type;        \
    ux_time_t timestamp;
#else
#define UXE_COMMON_FIELDS       \
    ux_atomic_t refcount;        \
    uxe_type type;        \
    ux_time_t timestamp;        \
    void* dummy;
#endif

struct ux_event_s {
  UXE_COMMON_FIELDS
};

UX_EXTERN size_t ux_event_size(uxe_type type);
UX_EXTERN ux_event_t* ux_event_malloc(uxe_type type);
UX_EXTERN ux_event_t* ux_event_ref(ux_event_t *e);
UX_EXTERN void ux_event_unref(ux_event_t *e);
UX_EXTERN ux_event_t* ux_event_clone(ux_event_t *e);

#ifdef __cplusplug
}
#endif

#endif // __UXE_H__
