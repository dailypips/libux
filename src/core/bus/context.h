/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <uv.h>
#include <ux/base/ux_memory.h>
#include <ux/bus/ux_context.h>
#include <ux/event/ux_event.h>
#include <ux/base/ux_logger.h>
#include <ux/base/ux_idarray.h>
#include <ux/domain/ux_provider.h>
#include "bar_generator.h"
#include "event_bus.h"
#include "hash.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ux_stop_s ux_stop_t;

#define LOOP_FIELDS                                                            \
  uv_mutex_t wait_mutex;                                                       \
  uv_cond_t wait_cond;                                                         \
  int stop_flag;                                                               \
  mpscq_t async_queue;

#define INSTRUMENT_MANAGER_FIELDS                                              \
  ux_atomic_t next_instrument_id;                                              \
  ux_idarray_t instrument_by_id;                                               \
  ux_shash_t *instrument_by_symbol;                                            \
  void *instruments[2];

#define ORDER_MANAGER_FIELDS                                                   \
  ux_atomic_t next_order_id;                                                   \
  ux_idarray_t order_by_id;                                                    \
  ux_shash_t *order_by_client_oid;                                             \
  ux_shash_t *order_by_provider_oid;                                           \
  ux_list_shash_t *order_list_by_oca;                                          \
  void *orders[2];

#define DATA_MANAGER_FIELDS                                                    \
  ux_idarray_t dm_asks;                                                        \
  ux_idarray_t dm_bids;                                                        \
  ux_idarray_t dm_trades;

struct ux_ctx_s {
  LOOP_FIELDS
  EVENT_BUS_FIELDS
  /* stat */
  uint64_t event_count;
  /* instrument manager*/
  INSTRUMENT_MANAGER_FIELDS
  ORDER_MANAGER_FIELDS
  /* bar factory */
  BAR_GENERATOR_MANAGER_PUBLIC_FIELDS
  // BAR_GENERATOR_PUBLIC_FIELDS
  DATA_MANAGER_FIELDS
  // STRATEGY_MANAGER_FIELDS
  /* simulator */

  ux_data_simulator_t *data_simulator;
  ux_execution_simulator_t *execution_simulator;
  /* logger */
  ux_event_logger_t *event_logger;
};

UX_FUNC void ux_ctx_init(ux_ctx_t *ctx);
UX_FUNC void ux_ctx_destroy(ux_ctx_t *ctx);
UX_FUNC void ux_ctx_clear(ux_ctx_t *ctx);

/* thread safe */
UX_EXTERN void ux_async_post(ux_ctx_t *ctx, ux_async_cb async_cb, void *data);


#ifdef __cplusplus
}
#endif

#endif // __CONTEXT_H__
