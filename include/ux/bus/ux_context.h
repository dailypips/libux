/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_CONTEXT_H__
#define __UX_CONTEXT_H__

#include <ux/base/ux_common.h>
#include <ux/base/ux_memory.h>
#include <ux/base/ux_datetime.h>
#include <ux/base/ux_currency.h>
#include <ux/base/ux_atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ux_ctx_s ux_ctx_t;
typedef struct ux_async_s ux_async_t;
typedef void (*ux_async_cb)(ux_ctx_t* ctx, void* data);

/* context module */
struct ux_async_s {
    ux_async_cb async_cb;
    void *data;
    /* private */
    ux_atomic_t next;
};

typedef enum {
    UX_RUN_DEFAULT = 0,
    UX_RUN_ONCE,
    UX_RUN_NOWAIT
} ux_run_mode;

UX_EXTERN ux_ctx_t* ux_ctx_new(void);
UX_EXTERN void ux_ctx_free(ux_ctx_t *ctx);
UX_EXTERN void ux_run(ux_ctx_t *ctx, ux_run_mode mode);
UX_EXTERN void ux_stop(ux_ctx_t *ctx);
UX_EXTERN void ux_wakeup(ux_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // __UX_CONTEXT_H__
