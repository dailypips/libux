/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "order.h"
#include "queue.h"
#include "atomic.h"

static void order_free(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    ux_order_destroy(order);
    ux_free(order);
}

void ux_order_init(ux_order_t *order)
{
    order->refcount = 1;
    order->id = -1;
}

#define if_free(x) if ((x)) ux_free((x));

void ux_order_destroy(ux_order_t *order)
{

}


void ux_order_ref(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    ux_atomic_full_fetch_add(&order->refcount, 1);
}

void ux_order_unref(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    if(ux_atomic_full_fetch_add(&order->refcount, -1) == 1) {
        order_free(order);
    }
}

#undef if_free
