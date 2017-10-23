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

void ux_stop_cancel(ux_stop_t *stop);
void ux_stop_disconnect(ux_stop_t *stop);

UX_FUNC void stop_on_bar(ux_stop_t *stop, uxe_bar_t *bar);
UX_FUNC void stop_on_bar_open(ux_stop_t *stop, uxe_bar_t *bar);
UX_FUNC void stop_on_trade(ux_stop_t *stop, uxe_trade_t *bar);
UX_FUNC void stop_on_ask(ux_stop_t *stop, uxe_ask_t *bar);
UX_FUNC void stop_on_bid(ux_stop_t *stop, uxe_bid_t *bar);

#ifdef __cplusplus
}
#endif

#endif // __STOP_H__
