/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_EVENT_FILTER_H__
#define __UX_EVENT_FILTER_H__

#include <ux/bus/ux_context.h>
#include <ux/event/ux_event.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ux_event_filter_s {
    ux_ctx_t *ctx;
    uint16_t event_mask;
    ux_event_t* (*filter)(ux_event_t *e);
}ux_event_filter_t;

int ux_register_event_filter(ux_ctx_t *ctx, ux_event_filter_t *filter);
int ux_unregister_event_filter(ux_ctx_t *ctx, ux_event_filter_t *filter);

#ifdef __cplusplus
}
#endif

#endif // __UX_EVENT_FILTER_H__
