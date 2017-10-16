/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_ACCOUNT_H__
#define __UX_ACCOUNT_H__

#include <ux/common.h>
#include <ux/types.h>
#include <ux/currency.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ux_account_s {
    ux_currency_t currency;
    void* positions[2];
    void* transactions[2];
    struct ux_account_t *parent;
    int update_parent;
};

#ifdef __cplusplus
}
#endif

#endif // __UX_ACCOUNT_H__
