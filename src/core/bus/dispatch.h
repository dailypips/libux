/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __DISPATCH_H__
#define __DISPATCH_H__

#include <uv.h>
#include <ux/base/ux_memory.h>
#include <ux/bus/ux_context.h>
#include <ux/event/ux_event.h>
#include <ux/base/ux_logger.h>
#include <ux/base/ux_idarray.h>
#include "bar_factory.h"
#include "event_bus.h"
#include "hash.h"

#ifdef __cplusplus
extern "C" {
#endif

/* dispatch module */
typedef enum { UX_DISPATCH_LATE = 0, UX_DISPATCH_IMMEDIATELY } ux_dispatch_mode;

UX_EXTERN void ux_dispatch_event(ux_ctx_t *ctx, ux_event_t *e,
                                 ux_dispatch_mode mode);
UX_FUNC void ux_emit_bufferd_events(ux_ctx_t *ctx);

#define DISPATCH_EVENT(e) ux_dispatch_event(ctx, (ux_event_t*)(e), UX_DISPATCH_IMMEDIATELY)
#define DISPATCH_EVENT_DELAY(e) ux_dispatch_event(ctx, (ux_event_t*)(e), UX_DISPATCH_LATE)

#ifdef __cplusplus
}
#endif

#endif // __DISPATCH_H__
