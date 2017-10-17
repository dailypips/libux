/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"
#include "queue.h"
#include "uthash.h"

static void instrument_free(ux_instrument_t *instrument)
{
    UX_ASSERT(instrument != NULL);
    ux_instrument_destory(instrument);
    ux_free(instrument);
}

void ux_instrument_init(ux_instrument_t *instrument)
{
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

#define if_free(x) if ((x)) ux_free((x));

void ux_instrument_destory(ux_instrument_t *instrument)
{
    if_free(instrument->symbol);
    if_free(instrument->description);
    if_free(instrument->exchange);
    if_free(instrument->price_format);
    if_free(instrument->formula);
    // free leg
    // TODO
}
#undef if_free

void ux_instrument_ref(ux_instrument_t *instrument)
{
    UX_ASSERT(instrument != NULL);
    ux_atomic_full_fetch_add(&instrument->refcount, 1);
}

void ux_instrument_unref(ux_instrument_t *instrument)
{
    UX_ASSERT(instrument != NULL);
    if(ux_atomic_full_fetch_add(&instrument->refcount, -1) == 1) {
        instrument_free(instrument);
    }
}

/* instrument manager */
ux_instrument_t *ux_get_instrument_by_id(ux_ctx_t *ctx, int id)
{
    khint_t iter = kh_get(int, ctx->instrument_by_id, id);
    if (iter == kh_end(ctx->instrument_by_id))
        return NULL;
    return kh_value(ctx->instrument_by_id, iter);
}

ux_instrument_t *ux_get_instrument_by_symbol(ux_ctx_t *ctx, const char *symbol)
{
    khint_t iter = kh_get(str, ctx->instrument_by_symbol, symbol);
    if (iter == kh_end(ctx->instrument_by_symbol))
        return NULL;
    return kh_value(ctx->instrument_by_symbol, iter);
}
