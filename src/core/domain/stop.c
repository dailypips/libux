/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

void ux_stop_init()
{

}

void ux_stop_destroy()
{

}

static void on_position(ux_stop_t *stop, ux_position_t *position);
static void on_bar(ux_stop_t *stop, uxe_bar_t *bar);
static void on_bar_open(ux_stop_t *stop, uxe_bar_t *bar);
static void on_trade(ux_stop_t *stop, uxe_trade_t *trade);
static void on_bid(ux_stop_t *stop, uxe_bid_t *bid);
static void on_ask(ux_stop_t *stop, uxe_ask_t *ask);

// virtaul double (*get_price)(double price);
// virtual double (*get_instrument_price)();
// virtual double (*get_stop_price)();
// virtual void (*cancel)();
// virtual void (*connect)();
// virtual void (*disconnect)();
