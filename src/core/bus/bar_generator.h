/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __BAR_GENERATOR_H__
#define __BAR_GENERATOR_H__

#include <ux/base/ux_common.h>
#include <ux/bus/ux_bar_generator.h>
#include "hash.h"

typedef struct ux_bar_factory_s ux_bar_factory_t;

UX_FUNC void ux_bar_generator_init(ux_bar_generator_t *item);
UX_FUNC void ux_bar_generator_destroy(ux_bar_generator_t *item);

#define BAR_GENERATOR_MANAGER_PUBLIC_FIELDS                                              \
  ux_list_ihash_t *list_by_instrument_id;                                       \
  void *reminder_items;

struct ux_bar_factory_s {
  BAR_GENERATOR_MANAGER_PUBLIC_FIELDS
};

UX_FUNC void ux_bar_factory_init(ux_ctx_t *ctx);
UX_FUNC void ux_bar_factory_destroy(ux_ctx_t *ctx);
UX_EXTERN void ux_bar_factory_add_generator(ux_ctx_t *ctx, ux_bar_generator_t *item);
UX_EXTERN void ux_bar_factory_remove_generator(ux_ctx_t *ctx,
                                         ux_bar_generator_t *generator);
UX_EXTERN void ux_bar_factory_add_reminder(ux_ctx_t *ctx,
                                          uxe_reminder_t *reminder);
UX_EXTERN void bar_factory_process_tick(ux_ctx_t *ctx, ux_event_tick_t *e);


#endif // __BAR_GENERATOR_H__
