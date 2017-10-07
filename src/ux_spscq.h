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
#include "ux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * lock-free spsc queue
 * modified from facebook folly library
 */

typedef struct ux_spscq_s ux_spscq_t;

struct ux_spscq_s {
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
UX_FUNC int spscq_init(ux_spscq_t *q, unsigned int size);

UX_FUNC void spscq_destroy(ux_spscq_t *q);

/* return 0 if successful
 * -1 means queue is full
 */
UX_FUNC int spscq_push(ux_spscq_t *q, void *e);

UX_FUNC void *spscq_pop(ux_spscq_t *q);

UX_FUNC void *spscq_peek(const ux_spscq_t *q);

// return 1 means empty
UX_FUNC int spscq_is_empty(ux_spscq_t *q);

// return 1 means full
UX_FUNC int spscq_is_full(ux_spscq_t *q);

UX_FUNC unsigned int spscq_size(ux_spscq_t *q);

UX_FUNC unsigned int spscq_capacity(ux_spscq_t *q);


#ifdef __cplusplus
}
#endif

#endif // __UX_SPSCQ_H__
