/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef EVENT_FILTER_H
#define EVENT_FILTER_H

#include <ux/context.h>

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

#endif // EVENT_FILTER_H