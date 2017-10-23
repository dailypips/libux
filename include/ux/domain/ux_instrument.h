/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_INSTRUMENT_H__
#define __UX_INSTRUMENT_H__

#include <ux/base/ux_type.h>
#include <ux/event/ux_event.h>

#ifdef __cplusplus
extern "C" {
#endif

/* instrument module */
typedef enum {
    UX_INSTRUMENT_TYPE_NONE = 0,
    UX_INSTRUMENT_TYPE_STOCK,
    UX_INSTRUMENT_TYPE_FUTURE,
    UX_INSTRUMENT_TYPE_OPTION,
    UX_INSTRUMENT_TYPE_FUTUREOPTION,
    UX_INSTRUMENT_TYPE_BOND,
    UX_INSTRUMENT_TYPE_FX,
    UX_INSTRUMENT_TYPE_INDEX,
    UX_INSTRUMENT_TYPE_ETF,
    UX_INSTRUMENT_TYPE_MULTILEG,
    UX_INSTRUMENT_TYPE_SYNTHETIC
}ux_instrument_type;

typedef enum {
    UX_PUT,
    UX_CALL
}ux_putcall_type;

typedef struct ux_instrument_alt_s {
    char *symbol;
    char *exchange;
    ux_currency_t currency;
}ux_instrument_alt_t;

typedef struct ux_instrument_leg_s {
    void *queue_node[2];
    ux_instrument_t *instrument;
    double weight;
}ux_instrument_leg_t;

struct ux_instrument_s {
    ux_atomic_t refcount;
    ux_iid_t id;
    ux_instrument_type type;
    char* symbol;
    char* description;
    char* exchange;
    ux_currency_t currency;
    ux_currency_t ccy1;
    ux_currency_t ccy2;
    double tickSize;
    ux_time_t maturity;
    double factor;
    double strike;
    ux_putcall_type putcall;
    double margin;
    char* price_format;
    char* formula;
    ux_instrument_alt_t alts[1024]; // max provider is 1024
    void* legs[2];
    uxe_trade_t *trade;
    uxe_ask_t *ask;
    uxe_bid_t *bid;
    uxe_bar_t *bar;
    struct ux_instrument_s *parent;
    int is_deleted    :1;
    int is_persistent :1;

    void* data_provider;
    void* execution_provider;
};

UX_EXTERN void ux_instrument_init(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_destroy(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_ref(ux_instrument_t *instrument);
UX_EXTERN void ux_instrument_unref(ux_instrument_t *instrument);

#ifdef __cplusplus
}
#endif

#endif // __UX_INSTRUMENT_H__
