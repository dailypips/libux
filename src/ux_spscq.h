/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_SPSCQ_H__
#define __UX_SPSCQ_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "ux_atomic.h"
#include "ux_mem.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__clang__) || defined(__GNUC__)
#define ALIGNED(size) __attribute__((__aligned__(size)))
#elif defined(__MSC_VER)
#define ALIGNED(size) __declspec(align(size))
#else
#error Cannot define ALIGNED on this platform
#endif

#define ALIGN_TO_AVOID_FALSE_SHARING ALIGNED(UX_CACHELINE_SIZE)

/*
 * lock-free spsc queue
 */

typedef struct ux_spscq_s ux_spscq_t;

struct ux_spscq_s {
    char pad1[UX_CACHELINE_SIZE];
    uint32_t capacity;
    void **records;
    ALIGN_TO_AVOID_FALSE_SHARING ux_atomic_t read_pos;
    ALIGN_TO_AVOID_FALSE_SHARING ux_atomic_t write_pos;
    char pad2[UX_CACHELINE_SIZE - sizeof(ux_atomic_t)];
};

/* @param size must be large than 2
 * @resule 0 successful
 */
int spsc_queue_init(ux_spscq_t *q, unsigned int size);

void spsc_queue_destroy(ux_spscq_t *q);

void spsc_queue_clear(ux_spscq_t *q);

/* return 0 if successful
 * -1 means queue is full
 */
int spsc_queue_push(ux_spscq_t *q, void *e);

void *spsc_queue_pop(ux_spscq_t *q);

void *spsc_queue_peek(const ux_spscq_t *q);

int spsc_queue_is_empty(ux_spscq_t *q);

int spsc_queue_is_full(ux_spscq_t *q);

unsigned int spsc_queue_size(ux_spscq_t *q);

unsigned int spsc_queue_capacity(ux_spscq_t *q);


#ifdef __cplusplus
}
#endif

#endif // __UX_SPSCQ_H__
