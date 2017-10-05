/******************************************************************************
 * Zero Quantitative Trading Library                                          *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 ******************************************************************************/

#include "ux_spscq.h"

int spscq_init(ux_spscq_t* q, uint32_t size)
{
    UX_ASSERT(size >= 2);
    q->records = ux_malloc(size * sizeof(void*));
    if (!q->records)
        return -1;

    q->capacity = size;
    ux_atomic_no_barrier_store(&q->read_pos, 0);
    ux_atomic_no_barrier_store(&q->write_pos, 0);

    return 0;
}

void spscq_destroy(ux_spscq_t* q)
{
    // No real synchronization needed at destructor time: only one
    // thread can be doing this.
    UX_ASSERT(spscq_is_empty(q) == 1);
    ux_free(q->records);
}

int spscq_push(ux_spscq_t* q, void* e)
{
    unsigned int currentWrite = ux_atomic_no_barrier_load(&q->write_pos);
    unsigned int nextRecord = currentWrite + 1;
    if (nextRecord == q->capacity)
        nextRecord = 0;

    if (nextRecord != ux_atomic_acq_load(&q->read_pos)) {
        q->records[currentWrite] = e;
        ux_atomic_rel_store(&q->write_pos, nextRecord);
        return 0;
    }

    // queue if full
    return -1;
}

void* spscq_pop(ux_spscq_t* q)
{
    unsigned int currentRead = ux_atomic_no_barrier_load(&q->read_pos);
    if (currentRead == ux_atomic_acq_load(&q->write_pos)) {
        // queue is empty
        return NULL;
    }

    unsigned int nextRecord = currentRead + 1;
    if (nextRecord == q->capacity)
        nextRecord = 0;
    ux_atomic_rel_store(&q->read_pos, nextRecord);

    return q->records[currentRead];
}

void* spscq_peek(const ux_spscq_t* q)
{
    unsigned int currentRead = ux_atomic_no_barrier_load(&q->read_pos);
    if (currentRead == ux_atomic_acq_load(&q->write_pos)) {
        // queue is empty
        return NULL;
    }

    return q->records[currentRead];
}

int spscq_is_empty(ux_spscq_t* q)
{
    return ux_atomic_acq_load(&q->read_pos) == ux_atomic_acq_load(&q->write_pos);
}

int spscq_is_full(ux_spscq_t* q)
{
    unsigned int nextRecord = ux_atomic_acq_load(&q->write_pos) + 1;
    if (nextRecord == q->capacity)
        nextRecord = 0;

    if (nextRecord != ux_atomic_acq_load(&q->read_pos)) {
        return 0;
    }

    // queue is full
    return 1;
}

unsigned int spscq_size(ux_spscq_t* q)
{
    int ret = ux_atomic_acq_load(&q->write_pos) - ux_atomic_acq_load(&q->read_pos);
    if (ret < 0)
        ret += q->capacity;
    return ret;
}

unsigned int spscq_capacity(ux_spscq_t* q) { return q->capacity; }
