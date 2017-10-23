/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __FILL_H__
#define __FILL_H__

#include <ux/domain/ux_fill.h>

#ifdef __cplusplus
extern "C" {
#endif

double ux_fill_get_value(ux_fill_t *fill);

#ifdef __cplusplus
}
#endif


#endif // __FILL_H__