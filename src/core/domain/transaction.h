/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <ux/domain/ux_transaction.h>

#ifdef __cplusplus
extern "C" {
#endif

void ux_transaction_add_fill(ux_transaction_t *transaction, ux_fill_t *fill);

#ifdef __cplusplus
}
#endif

#endif // __TRANSACTION_H__
