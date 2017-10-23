/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "pricer.h"
#include "position.h"
#include "instrument.h"

static double get_price(ux_pricer_t *pricer, ux_position_t *position)
{
    default_pricer_t *p = (default_pricer_t*)pricer;
    if (position->side == UX_POSITION_SIDE_LONG) {
        uxe_bid_t *bid = position->instrument->bid;
        if (bid)
            return bid->price;
    }else {
        uxe_ask_t *ask = position->instrument->ask;
        if (ask)
            return ask->price;
    }
    uxe_trade_t *trade = position->instrument->trade;
    if (trade)
        return trade->price;
    uxe_bar_t *bar = position->instrument->bar;
    if (bar)
        return bar->close;
    return position->avg_price;
}

static double get_value(ux_pricer_t *pricer, ux_position_t *position)
{
    default_pricer_t *p = (default_pricer_t*)pricer;
    if (position->instrument->factor != 0.0)
        return get_price(pricer, position) * position->amount * position->instrument->factor;
    else
        return get_price(pricer, position) * position->amount;
}

static void default_pricer_free(default_pricer_t *pricer)
{
    ux_free(pricer);
}

default_pricer_t * ux_default_pricer_new(ux_ctx_t *ctx)
{
    default_pricer_t *pricer = ux_zalloc(sizeof(*pricer));
    pricer->free = (void (*)(ux_pricer_t*))default_pricer_free;
    pricer->get_price = (double (*)(ux_pricer_t*, ux_position_t*))get_price;
    pricer->get_value = (double (*)(ux_pricer_t*, ux_position_t*))get_value;

    return pricer;
}


