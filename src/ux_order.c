/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

static void order_free(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    ux_order_destory(order);
    ux_free(order);
}

UX_EXTERN void ux_order_init(ux_order_t *order)
{

}

UX_EXTERN void ux_order_destory(ux_order_t *order)
{

}

UX_EXTERN void ux_order_ref(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    ux_atomic_full_fetch_add(&order->refcount, 1);
}

UX_EXTERN void ux_order_unref(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    if(ux_atomic_full_fetch_add(&order->refcount, -1) == 1) {
        order_free(order);
    }
}
