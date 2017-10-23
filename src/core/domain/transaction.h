/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <ux/domain/ux_strategy.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ux_transaction_s {
    void *queue_node[2];
    double qty;
    double price;
    double commission;
    double is_done;
    void*  fills[2];
};

#ifdef __cplusplus
}
#endif

#endif // __TRANSACTION_H__
