/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __STOP_H__
#define __STOP_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_stop.h>

#ifdef __cplusplus
extern "C" {
#endif

void ux_stop_init(ux_stop_t *stop);
void ux_stop_destroy(ux_stop_t *stop);

#ifdef __cplusplus
}
#endif

#endif // __STOP_H__
