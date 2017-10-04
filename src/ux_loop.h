/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_LOOP_H__
#define __UX_LOOP_H__

#include "ux_spscq.h"
#include "ux_mpscq.h"
#include <uv.h>
#include <stddef.h>
#include "ux_bus.h"

typedef struct ux_loop_s ux_loop_t;
typedef struct ux_async_s ux_async_t;

typedef void (*ux_async_cb)(ux_loop_t *loop, void *data);

struct ux_async_s {
    ux_async_cb async_cb;
    void *data;
    ux_loop_t *loop;
    ux_mpscq_node mpsc_node;
};

struct ux_loop_s {
    uv_mutex_t wait_mutex;
    uv_cond_t  wait_cond;
    int stop_flag;
    ux_mpscq_t async_queue;
    ux_bus_t bus;
};

typedef enum {
    UX_RUN_DEFAULT = 0,
    UX_RUN_ONCE,
    UX_RUN_NOWAIT
}ux_run_mode;

void ux_loop_init(ux_loop_t *loop);
void ux_loop_destory(ux_loop_t *loop);
void ux_loop_run(ux_loop_t *loop, ux_run_mode mode);
void ux_loop_stop(ux_loop_t *loop);
void ux_loop_wakeup(ux_loop_t* loop);

/* thread safe */
void ux_async_post(ux_loop_t *loop, ux_async_cb async_cb, void *data);

void ux_loop_add_queue(ux_loop_t *loop, void *q);
void ux_loop_remove_queue(ux_loop_t *loop, void *q);

#endif // __UX_LOOP_H__
