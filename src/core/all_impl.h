#include <ux/event_def.h>

#define event_default_destory NULL
#define event_default_dispatch NULL
#define event_default_clone NULL

#define EVENT_DESTORY_IMPL(name, lname, edestory, eclone, edispatch) void event_##edestory##_destory(ux_event_t *e) {}
EVENTDEF(EVENT_DESTORY_IMPL)
#undef EVENT_DESTORY_IMPL

#define EVENT_CLONE_IMPL(name, lname, edestory, eclone, edispatch) void event_##eclone##_clone(ux_event_t *e) {}
EVENTDEF(EVENT_CLONE_IMPL)
#undef EVENT_CLONE_IMPL


#define EVENT_DISPATCH_IMPL(name, lname, edestory, eclone, edispatch) void event_##edispatch##_dispatch(ux_ctx_t *ctx, ux_event_t *e) {}
EVENTDEF(EVENT_DISPATCH_IMPL)
#undef EVENT_DISPATCH_IMPL

