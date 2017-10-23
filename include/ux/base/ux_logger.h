/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_LOGGER_H__
#define __UX_LOGGER_H__

#include <ux/base/ux_type.h>

#ifdef __cplusplus
extern "C" {
#endif

struct uxe_logger_s {
    void (*init)(ux_ctx_t *ctx, const char *name);
    void (*start)(ux_event_logger_t *logger);
    void (*stop)(ux_event_logger_t *logger);
    void (*on_event)(ux_event_logger_t *logger, ux_event_t *e);
    void (*close)(ux_event_logger_t *logger);
};


#ifdef __cplusplus
}
#endif

#endif // __UX_LOGGER_H__
