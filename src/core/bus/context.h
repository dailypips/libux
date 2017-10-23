/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <uv.h>
#include <ux/base/ux_idarray.h>
#include <ux/base/ux_logger.h>
#include <ux/base/ux_type.h>
#include <ux/bus/ux_context.h>
#include <ux/domain/ux_provider.h>
#include <ux/event/ux_event.h>
#include "hash.h"
#include "bar_factory.h"
#include "data_manager.h"
#include "event_bus.h"
#include "instrument_manager.h"
#include "order_manager.h"
#include "subscription_manager.h"


#ifdef __cplusplus
extern "C" {
#endif

struct ux_ctx_s {
  /* loop */
  uv_mutex_t wait_mutex;
  uv_cond_t wait_cond;
  int stop_flag;
  mpscq_t async_queue;
  EVENT_BUS_FIELDS
  /* stat */
  uint64_t event_count;
  /* instrument manager*/
  INSTRUMENT_MANAGER_FIELDS
  ORDER_MANAGER_FIELDS
  /* bar factory */
  BAR_FACTORY_FIELDS

  DATA_MANAGER_FIELDS
  SUBSCRIPTION_MANANGER_FIELDS
  // STRATEGY_MANAGER_FIELDS
  /* simulator */

  ux_data_simulator_t *data_simulator;
  ux_execution_simulator_t *execution_simulator;
  /* logger */
  ux_event_logger_t *event_logger;
  /* currency converter */
  currency_converter_t *currency_converter;
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
