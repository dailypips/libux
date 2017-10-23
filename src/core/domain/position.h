/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __POSITION_H__
#define __POSITION_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_position.h>

#ifdef __cplusplus
extern "C" {
#endif

double ux_position_get_value(ux_position_t *position);
double ux_position_get_price(ux_position_t *position);
void ux_position_add_fill(ux_position_t *position, ux_fill_t *fill);

#ifdef __cplusplus
}
#endif

#endif // __POSITION_H__
