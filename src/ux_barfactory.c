/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

typedef struct ux_barfactory_s ux_barfactory_t;

typedef struct ux_bar_item_s {
    ux_barfactory_t* factory;
    ux_instrument_t* instrument;
    int provider_id;
    ux_bar_type bar_type;
    ux_bar_input bar_input;
    long bar_size;
    int session_enabled;
    ux_timespan_t session1;
    ux_timespan_t session2;
} ux_bar_item_t;

struct ux_barfactory_s {
    ux_loop_t *framework;
    void* item_by_instrument_id;
    void* reminder_items;
};
