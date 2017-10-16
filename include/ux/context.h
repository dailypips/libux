/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_CONTEXT_H__
#define __UX_CONTEXT_H__

#include <ux/common.h>
#include <ux/memory.h>
#include <ux/datetime.h>
#include <ux/currency.h>
#include <ux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* context module */
struct ux_async_s {
    ux_async_cb async_cb;
    void *data;
    /* private */
    ux_atomic_t next;
};

UX_EXTERN ux_ctx_t* ux_ctx_new(void);
UX_EXTERN void ux_ctx_free(ux_ctx_t *ctx);
UX_EXTERN void ux_run(ux_ctx_t *ctx, ux_run_mode mode);
UX_EXTERN void ux_stop(ux_ctx_t *ctx);
UX_EXTERN void ux_wakeup(ux_ctx_t *ctx);

#ifdef __cplusplus
}
#endif

#endif // __UX_CONTEXT_H__
