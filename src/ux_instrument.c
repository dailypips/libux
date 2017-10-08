/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

static void instrument_free(ux_instrument_t *instrument)
{
    UX_ASSERT(instrument != NULL);
    ux_instrument_destory(instrument);
    ux_free(instrument);
}

UX_EXTERN void ux_instrument_init(ux_instrument_t *instrument)
{

}

UX_EXTERN void ux_instrument_destory(ux_instrument_t *instrument)
{

}

UX_EXTERN void ux_instrument_ref(ux_instrument_t *instrument)
{
    UX_ASSERT(instrument != NULL);
    ux_atomic_full_fetch_add(&instrument->refcount, 1);
}

UX_EXTERN void ux_instrument_unref(ux_instrument_t *instrument)
{
    UX_ASSERT(instrument != NULL);
    if(ux_atomic_full_fetch_add(&instrument->refcount, -1) == 1) {
        instrument_free(instrument);
    }
}
