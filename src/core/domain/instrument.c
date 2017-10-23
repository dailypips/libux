/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include <ux/base/ux_type.h>
#include <ux/event/ux_events.h>
#include "instrument.h"
#include "hash.h"
#include "order.h"
#include "queue.h"
#include "atomic.h"

static void instrument_free(ux_instrument_t *instrument) {
  UX_ASSERT(instrument != NULL);
  ux_instrument_destroy(instrument);
  ux_free(instrument);
}

void ux_instrument_init(ux_instrument_t *instrument) {
  instrument->refcount = 1;
  instrument->id = -1;
  instrument->symbol = NULL;
  instrument->description = NULL;
  instrument->exchange = NULL;
  instrument->currency = UX_CURRENCY_USD;
  instrument->price_format = NULL;
  instrument->formula = NULL;
  instrument->trade = NULL;
  instrument->bid = NULL;
  instrument->ask = NULL;
  instrument->bar = NULL;
  instrument->parent = NULL;
  instrument->data_provider = NULL;
  instrument->execution_provider = NULL;

  QUEUE_INIT(&instrument->legs);
}

#define if_free(x)                                                             \
  if ((x))                                                                     \
    ux_free((x));

void ux_instrument_destroy(ux_instrument_t *instrument) {
  if_free(instrument->symbol);
  if_free(instrument->description);
  if_free(instrument->exchange);
  if_free(instrument->price_format);
  if_free(instrument->formula);

  for (int i = 0; i < UX_PID_MAX; i++) {
    if_free(instrument->alts[i].symbol);
    if_free(instrument->alts[i].exchange);
  }

  // TODO:优化，直接规定最多32个LEGS

  QUEUE *q;
  while (!QUEUE_EMPTY(&instrument->legs)) {
    q = QUEUE_HEAD(&instrument->legs);
    ux_instrument_leg_t *leg = QUEUE_DATA(q, ux_instrument_leg_t, queue_node);
    ux_instrument_unref(leg->instrument);
    QUEUE_REMOVE(q);
    ux_free(leg);
  }
}

void ux_instrument_ref(ux_instrument_t *instrument) {
  UX_ASSERT(instrument != NULL);
  ux_atomic_full_fetch_add(&instrument->refcount, 1);
}

void ux_instrument_unref(ux_instrument_t *instrument) {
  UX_ASSERT(instrument != NULL);
  if (ux_atomic_full_fetch_add(&instrument->refcount, -1) == 1) {
    instrument_free(instrument);
  }
}

const char *ux_instrument_get_symbol(ux_instrument_t *instrument,
                                     ux_pid_t provider_id) {
  UX_ASSERT(provider_id < UX_PID_MAX);
  if (instrument->alts[provider_id].symbol != NULL)
    return instrument->alts[provider_id].symbol;
  return instrument->symbol;
}

const char* ux_instrument_get_exchange(ux_instrument_t *instrument,
                                       ux_pid_t provider_id) {
  UX_ASSERT(provider_id < UX_PID_MAX);
  if (instrument->alts[provider_id].exchange != NULL)
    return instrument->alts[provider_id].exchange;
  return instrument->exchange;
}

ux_currency_t ux_instrument_get_currency(ux_instrument_t *instrument,
                                         ux_pid_t provider_id) {
  UX_ASSERT(provider_id < UX_PID_MAX);
  if (instrument->alts[provider_id].currency != UX_CURRENCY_NONE)
    return instrument->alts[provider_id].currency;
  return instrument->currency;
}

ux_instrument_leg_t* ux_instrument_add_leg(ux_instrument_t *instrument, ux_instrument_t *leg,
                           double weight) {
  ux_instrument_leg_t *node = ux_zalloc(sizeof(*node));
  ux_instrument_ref(leg);
  node->instrument = leg;
  node->weight = weight;
  QUEUE_INSERT_TAIL(&instrument->legs, &node->queue_node);
}

void ux_instrument_remove_leg(ux_instrument_t *instrument, ux_instrument_leg_t *leg)
{
    UX_UNUSED(instrument);
    QUEUE_REMOVE(&leg->queue_node);
}

static inline void destory_alt(ux_instrument_alt_t *alt)
{
    if_free(alt->symbol);
    if_free(alt->exchange);
    alt->currency = UX_CURRENCY_NONE;
}

void ux_instrument_add_alt(ux_instrument_t *instrument, ux_pid_t provider_id, const char *symbol, const char *exchange, ux_currency_t currency)
{
    ux_instrument_alt_t *alt = &instrument->alts[provider_id];
    destory_alt(alt);
    alt->symbol = ux_strdup(symbol);
    alt->exchange = ux_strdup(exchange);
    alt->currency = currency;
}

void ux_instrument_remove_alt(ux_instrument_t *instrument, ux_pid_t provider_id)
{
    ux_instrument_alt_t *alt = &instrument->alts[provider_id];
    destory_alt(alt);
}

#undef if_free
