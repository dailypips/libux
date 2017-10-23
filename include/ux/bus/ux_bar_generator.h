/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_BAR_GENERATOR_H__
#define __UX_BAR_GENERATOR_H__

#include <ux/base/ux_common.h>
#include <ux/base/ux_currency.h>
#include <ux/base/ux_datetime.h>
#include <ux/event/tick.h>
#include <ux/event/ux_event.h>
#include <ux/domain/ux_order.h>

#ifdef __cplusplus
extern "C" {
#endif

/* bar generator */
typedef struct ux_bar_generator_s ux_bar_generator_t;

typedef void (*ux_on_tick_cb)(ux_bar_generator_t *generator, ux_event_tick_t *tick);

#define UX_BAR_GENERATOR_PUBLIC_FIELDS                                         \
  /* private */                                                                \
  void *queue_node[2];                                                         \
  /* read only */                                                              \
  ux_ctx_t *ctx;                                                               \
  ux_instrument_t *instrument;                                                 \
  ux_bar_input bar_input;                                                      \
  ux_on_tick_cb on_tick;                                                       \
  uxe_bar_t *bar;

typedef struct ux_bar_generator_s {
  UX_BAR_GENERATOR_PUBLIC_FIELDS
} ux_bar_generator_t;

#ifdef __cplusplus
}
#endif

#endif // __UX_BAR_GENERATOR_H__
