/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_internal.h"

typedef struct {
    ux_order_status status;
    double avg_price;
    double cum_qty;
    double leaves_qty;
    double commission;
}node_t;

typedef struct ux_default_execution_simulator_s {
    UX_EXECUTION_SIMULATOR_PUBLIC_FIELDS
    void* orders[2];
    ux_timespan_t auction1;
    ux_timespan_t auction2;
}ux_default_execution_simulator_t;


/* provider */
static int is_enable(ux_provider_t *provider)
{

}

static int get_id(ux_provider_t *provider)
{

}

const char* get_name(ux_provider_t *provider)
{

}

static ux_provider_status get_status(ux_provider_t *provider)
{

}

static void connect(ux_provider_t *provider, ux_provider_cb on_connect)
{

}

static void disconnect(ux_provider_t *provider, ux_provider_cb on_disconnect)
{

}

/* execution provider */
static void send(ux_event_execution_command_t command)
{

}

/* execution simulator */
static void on_bid(ux_event_bid_t *bid)
{

}

static void on_ask(ux_event_bid_t *ask)
{

}

static void on_trade(ux_event_on_trade_t *trade)
{

}

static void on_level2_snapshot(ux_event_l2snapshot_t *snapshot)
{

}

static void on_level2_update(ux_event_l2update_t *update)
{

}

static void on_bar_open(ux_event_bar_t *bar)
{

}

static void on_bar(ux_event_bar_t *bar)
{

}

static void clear(void)
{

}


void ux_default_execution_simulator_init(ux_default_execution_simulator_t *simulator)
{
    simulator->is_enable = is_enable;
    simulator->get_id = get_id;
    simulator->get_name = get_name;
    simulator->get_status = get_status;
    simulator->connect = connect;
    simulator->disconnect = disconnect;
    simulator->send = send;
    simulator->on_bid = on_bid;
    simulator->on_ask = on_ask;
    simulator->on_trade = on_trade;
    simulator->on_level2_snapshot = on_level2_snapshot;
    simulator->on_level2_update = on_level2_update;
    simulator->on_bar_open = on_bar_open;
    simulator->on_bar = on_bar;
    simulator->clear = clear;
}
