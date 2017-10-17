/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_BARFACTORY_H__
#define __UX_BARFACTORY_H__

#include <ux/common.h>
#include <ux/datetime.h>
#include <ux/currency.h>
#include <ux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* barfactory */
typedef struct ux_barfactory_item_s ux_barfactory_item_t;
typedef void (*ux_on_tick_cb)(ux_barfactory_item_t *item, ux_event_tick_t *tick);

#define UX_BAR_ITEM_PUBLIC_FIELDS \
    /* private */ \
    void* queue_node[2];   \
    /* read only */ \
    ux_ctx_t *ctx;   \
    ux_instrument_t* instrument;    \
    ux_bar_input bar_input; \
    ux_on_tick_cb on_tick;  \
    uxe_bar_t *bar;


#define UX_BAR_ITEM_COMMON_FILEDS \
    int provider_id;    \
    ux_bar_type bar_type;   \
    long bar_size;  \
    int session_enabled;    \
    ux_timespan_t session1; \
    ux_timespan_t session2;


struct ux_barfactory_item_s {
    UX_BAR_ITEM_PUBLIC_FIELDS
};

#ifdef __cplusplus
}
#endif

#endif // __UX_BARFACTORY_H__
