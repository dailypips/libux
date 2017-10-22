/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UXE_LOGGER_H__
#define __UXE_LOGGER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uxe_logger_s ux_event_logger_t;

typedef struct ux_ctx_s ux_ctx_t;

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

#endif // __UXE_LOGGER_H__
