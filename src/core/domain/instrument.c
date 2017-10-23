/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "instrument.h"
#include "order.h"
#include "queue.h"
#include "hash.h"

static void instrument_free(ux_instrument_t *instrument)
{
    UX_ASSERT(instrument != NULL);
    ux_instrument_destroy(instrument);
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

void ux_instrument_destroy(ux_instrument_t *instrument)
{
    if_free(instrument->symbol);
    if_free(instrument->description);
    if_free(instrument->exchange);
    if_free(instrument->price_format);
    if_free(instrument->formula);

    for (int i = 0; i < 1024; i++) {
        if (instrument->alts[i] != NULL) {
            if_free(instrument->alts[i].symbol);
            if_free(instrument->alts[i].exchange);
        }
    }

    //TODO:优化，直接规定最多32个LEGS

    QUEUE* q;
    while(!QUEUE_EMPTY(&instrument->legs)) {
        q = QUEUE_HEAD(&instrument->legs);
        ux_instrument_t* inst = QUEUE_DATA(q, ux_instrument_leg_t, instrument);
        ux_instrument_unref(inst);
        QUEUE_REMOVE(q);
        ux_instrument_leg_t *node = container_of(q, ux_instrument_leg_t, queue_node);
        ux_free(node);
    }
}


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

const char* ux_instrument_get_symbol(ux_instrument_t *instrument, int provider_id)
{
    UX_ASSERT(provider_id < 1024);
    if (instrument->alts[provider_id].symbol != NULL)
        return instrument->alts[provider_id].symbol;
    return instrument->symbol;
}

const char* ux_instrument_get_exchange(ux_instrument_t *instrument, int provider_id)
{
    UX_ASSERT(provider_id < 1024);
    if (instrument->alts[provider_id].exchange != NULL)
        return instrument->alts[provider_id].exchange;
    return instrument->exchange;
}

ux_currency_t ux_instrument_get_exchange(ux_instrument_t *instrument, int provider_id)
{
    UX_ASSERT(provider_id < 1024);
    if (instrument->alts[provider_id].currency != 0)
        return instrument->alts[provider_id].currency;
    return instrument->currency;
}

void ux_instrument_add_leg(ux_instrument_t *instrument, ux_instrument_t *leg, double weight)
{
    ux_instrument_leg_t *node = ux_zalloc(sizeof(*node));
    ux_instrument_ref(leg);
    node->instrument = leg;
    node->weight = weight;
    QUEUE_INSERT_TAIL(&instrument->legs, node);
}

struct queue_node_t {
    void *queue_node[2];
    void *event;
};

static void order_free(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    ux_order_destroy(order);
    ux_free(order);
}

UX_EXTERN void ux_order_init(ux_order_t *order)
{
    order->refcount = 1;
    order->id = -1;
    order->clId = -1;
    order->algoId = -1;
    order->providerOrderId = NULL;
    order->clOrderId = NULL;
    order->instrumentId = -1;
    order->portfolioId = -1;
    order->strategyId = -1;
    order->execInst = NULL;
    order->oca = NULL;
    order->text = NULL;

    QUEUE_INIT(&order->commands);
    QUEUE_INIT(&order->reports);
}

UX_EXTERN void ux_order_destroy(ux_order_t *order)
{
    if_free(order->providerOrderId);
    if_free(order->clOrderId);
    if_free(order->execInst);
    if_free(order->oca);
    if_free(order->text);
    if_free(order->account);
    if_free(order->clientID);

    QUEUE* q;
    while(!QUEUE_EMPTY(&order->commands)) {
        q = QUEUE_HEAD(&order->commands);
        ux_event_t* e = QUEUE_DATA(q, queue_node_t, event);
        ux_event_unref(e);
        QUEUE_REMOVE(q);
        struct queue_node_t *node = container_of(q, queue_node_t, queue_node);
        ux_free(node);
    }

    while(!QUEUE_EMPTY(&order->reports)) {
        q = QUEUE_HEAD(&order->reports);
        ux_event_t* e = QUEUE_DATA(q, queue_node_t, event);
        ux_event_unref(e);
        QUEUE_REMOVE(q);
        struct queue_node_t *node = container_of(q, queue_node_t, queue_node);
        ux_free(node);
    }
}

UX_EXTERN void ux_order_ref(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    ux_atomic_full_fetch_add(&order->refcount, 1);
}

UX_EXTERN void ux_order_unref(ux_order_t *order)
{
    UX_ASSERT(order != NULL);
    if(ux_atomic_full_fetch_add(&order->refcount, -1) == 1) {
        order_free(order);
    }
}

UX_EXTERN void ux_order_add_command(ux_order_t *order, uxe_execution_command_t *command)
{
    struct queue_node_t *node = ux_zalloc(sizeof(*node)); // queue node size
    node->event = command;
    QUEUE_INSERT_TAIL(&order->commands, &node->queue_node);
    ux_event_ref((ux_event_t*)command);
}

UX_EXTERN void ux_order_add_report(ux_order_t *order, uxe_execution_report_t *report)
{
    struct queue_node_t *node = ux_zalloc(sizeof(*node)); // queue node size
    node->event = report;
    QUEUE_INSERT_TAIL(&order->reports, &node->queue_node);
    ux_event_ref((ux_event_t*)report);
}

UX_EXTERN void ux_order_on_execution_report(ux_order_t *order, uxe_execution_report_t *report)
{
    order->orderStatus = report->order_status;
    if (report->exec_type == UX_EXEC_TRADE)
        order->avgPx = (order->avgPx * order->cumQty + report->last_price *  report->last_qty) /
                (order->cumQty + report->last_qty);
    order->cumQty = report->cum_qty;
    order->leavesQty = report->leaves_qty;
    if (report->exec_type == UX_EXEC_NEW)
        report->provider_order_id = ux_strdup(report->provider_order_id);
    if (report->exec_type == UX_EXEC_REPLACE) {
        order->orderType = report->order_type;
        order->price = report->price;
        order->stopPx = report->stop_price;
        order->qty = report->order_qty;
    }
    ux_order_add_report(order, report);
}

#undef if_free
