/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

typedef struct ux_default_data_simulator_s ux_default_data_simulator_t;

static const char *data_simulator_name = "DataSimulator";
static const char *data_simulator_desc = "Default data simulator";
static const char *data_simulator_url = "www.dailypips.org";

struct ux_default_data_simulator_s {
    UX_DATA_SIMULATOR_PUBLIC_FIELDS
    char *name;
    char *description;
    char *url;
    ux_provider_status status;
    int enabled;
};

static ux_provider_status get_status(ux_provider_t *provider)
{
    ux_default_data_simulator_t *simulator = (ux_default_data_simulator_t*)provider;
    return simulator->status;
}

static void set_status(ux_provider_t *provider, ux_provider_status status)
{
    ux_default_data_simulator_t *simulator = (ux_default_data_simulator_t*)provider;
    if (status != simulator->status) {
        ux_provider_status old = simulator->status;
        simulator->status = status;
        // notify status changed
        // TODO
    }
}

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
    ux_default_data_simulator_t *simulator = (ux_default_data_simulator_t*)provider;
    if(simulator->enabled && simulator->status == UX_PROVIDER_STATUS_DISCONNECTED) {
        set_status(provider, UX_PROVIDER_STATUS_CONNECTED);
        on_connect(provider, UX_OK);
    }
}

static void disconnect(ux_provider_t *provider, ux_provider_cb on_disconnect)
{

}

static void subscribe(ux_data_provider_t *provider, ux_instrument_t *instrument, int level2_flag, ux_provider_cb on_subscribe)
{

}

static void unsubscribe(ux_data_provider_t *provider, ux_instrument_t *instrument, ux_provider_cb on_unsubscribe)
{

}

static void run(ux_data_simulator_t *simulator)
{

}

static void clear(ux_data_simulator_t *simulator)
{

}

void ux_default_data_simulator_init(ux_default_data_simulator_t *simulator)
{
    /* set callbacks */
    simulator->is_enable = is_enable;
    simulator->get_id = get_id;
    simulator->get_name = get_name;
    simulator->get_status = get_status;
    simulator->connect = connect;
    simulator->disconnect = disconnect;
    simulator->subscribe = subscribe;
    simulator->unsubscribe = unsubscribe;
    simulator->run = run;
    simulator->clear = clear;
    simulator->name =data_simulator_name;
    simulator->description = data_simulator_desc;
    /* set info */
    simulator->url = data_simulator_url;
    simulator->time1 = MIN_DATE_TIME;
    simulator->time2 = MAX_DATE_TIME;
}

void ux_default_data_simulator_destroy(ux_default_data_simulator_t *simulator)
{

}
