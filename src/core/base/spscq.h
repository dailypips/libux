/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __SPSCQ_H__
#define __SPSCQ_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "atomic.h"
#include <ux/base/ux_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * lock-free spsc queue
 * modified from facebook folly library
 */

typedef struct spscq_s spscq_t;

struct spscq_s {
    char pad1[UX_CACHELINE_SIZE];
    uint32_t capacity;
    void **records;
    UX_ALIGN(UX_CACHELINE_SIZE) ux_atomic_t read_pos;
    UX_ALIGN(UX_CACHELINE_SIZE) ux_atomic_t write_pos;
    char pad2[UX_CACHELINE_SIZE - sizeof(ux_atomic_t)];
};

/* @param size must be large than 2
 * @resule 0 successful
 */
UX_FUNC int spscq_init(spscq_t *q, unsigned int size);

UX_FUNC void spscq_destroy(spscq_t *q);

/* return 0 if successful
 * -1 means queue is full
 */
UX_FUNC int spscq_push(spscq_t *q, void *e);

UX_FUNC void *spscq_pop(spscq_t *q);

UX_FUNC void *spscq_peek(const spscq_t *q);

// return 1 means empty
UX_FUNC int spscq_is_empty(spscq_t *q);

// return 1 means full
UX_FUNC int spscq_is_full(spscq_t *q);

UX_FUNC unsigned int spscq_size(spscq_t *q);

UX_FUNC unsigned int spscq_capacity(spscq_t *q);


#ifdef __cplusplus
}
#endif

#endif // __SPSCQ_H__
