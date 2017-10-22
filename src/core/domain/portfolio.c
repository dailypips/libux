/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_internal.h"

void ux_portfolio_init(ux_ctx_t *ctx, ux_portfolio_t *portfolio)
{
    portfolio->ctx = ctx;
    portfolio->id = -1;
    portfolio->name = NULL;
    portfolio->description = NULL;
    portfolio->parent = NULL;
    //TODO:other
    QUEUE_INIT(&portfolio->children);
    QUEUE_INIT(&portfolio->transactions);
    QUEUE_INIT(&portfolio->positions);
    kv_init(&portfolio->transaction_by_order_id);
    kv_init(&portfolio->position_by_instrument_id);
    ux_account_init(&portfolio->account);
}

void ux_portfolio_destroy(ux_portfolio_t *portfolio)
{

}

double ux_portfolio_get_active_orders_value(ux_portfolio_t *portfolio)
{

}

ux_position_t* ux_portfolio_get_position_by_instrument_id(ux_portfolio_t *portfolio, int instrument_id)
{
    ux_position_t *position = (ux_position_t*)kv_get(&portfolio->position_by_instrument_id, instrument_id);
    return position;
}

static ux_position_t* get_or_create_position_by_instrument(ux_portfolio_t *portfolio, ux_instrument_t *instrument)
{
    ux_position_t *position = ux_portfolio_get_position_by_instrument_id(portfolio, instrument->id);
    if (!position) {
        position = ux_zalloc(sizeof(ux_position_t));
        QUEUE_INSERT_TAIL(&portfolio->positions, &position->queue_node);
        kv_set(&portfolio->position_by_instrument_id, instrument->id, (uintptr_t)position);
    }
    return position;
}

ux_transaction_t* ux_portfolio_get_transaction_by_order_id(ux_portfolio_t *portfolio, int order_id)
{
    ux_transaction_t *transaction = (ux_transaction_t*)kv_get(&portfolio->transaction_by_order_id, order_id);
    return transaction;
}

void ux_portfolio_add_transaction(ux_portfolio_t *portfolio, ux_transaction_t *transacation, int order_id, int queued)
{
    QUEUE_INSERT_TAIL(&portfolio->transactions, &transacation->queue_node);
    kv_set(&portfolio->transaction_by_order_id, order_id, (uintptr_t)transacation);
    if(portfolio->parent && portfolio->update_parent)
        ux_portfolio_add_transaction(portfolio->parent, transacation, order_id, queued);
}

void ux_portfolio_add_fill(ux_portfolio_t *portfolio, ux_fill_t *fill, int queued)
{
    //TODO: fills.add(fill)
    ux_event_t *e = ux_event_malloc(UXE_ON_FILL);
    ((uxe_on_fill_t*)e)->portfolio = portfolio;
    ((uxe_on_fill_t*)e)->fill = fill;
    ux_dispatch_event(portfolio->ctx, e, queued);
    int new_position_flag = 0;
    ux_position_t *position = ux_portfolio_get_position_by_instrument_id(portfolio, fill->instrument->id);
    if (!position)
        position = get_or_create_position_by_instrument(portfolio, fill->instrument);
    if(position->amount == 0.0) // TODO
        new_position_flag = 1;
    ux_position_add_fill(position, fill);
    ux_account_add_fill(&portfolio->account, fill, 0);
    if (new_position_flag) {
        //TODO
        // portfolioStatistics.OnPositionOpened(position);
        //eventServer.OnPositionOpened(this, position, queued);
    }else {
        if(fill->qty > fabs(position->amount) && position->amount != 0.0 &&
                ((fill->side == UX_ORDER_SIDE_BUY && position->side == UX_POSITION_SIDE_LONG) ||
                 (fill->side == UX_ORDER_SIDE_SELL && position->side == UX_POSITION_SIDE_SHORT))) {
            ;//portfolioStatistics.OnPositionSideChanged(position);
        }
        if (position->amount != 0.0) {
            //portfolioStatistics.OnPositionChanged(position);
        }
        e = ux_event_malloc(UXE_POSITION_CHANGED);
        ((uxe_position_changed_t*)e)->portfolio = portfolio;
        ((uxe_position_changed_t*)e)->position = position;
        ux_dispatch_event(portfolio->ctx, e, queued);
        if (position->amount == 0.0) {
            //TODO:
            //this.portfolioStatistics.OnPositionClosed(position);
           // this.framework.eventServer.EmitOnPositionClosed(this, position, queued);
        }
    }

    if (portfolio->parent && portfolio->update_parent)
        ux_portfolio_add_fill(portfolio->parent, fill, queued);
}

static void emit_on_transaction(ux_portfolio_t *portfolio, ux_transaction_t *transaction, int queued)
{
    ux_event_t *e = ux_event_malloc(UXE_ON_TRANSACETION);
    ((uxe_on_transaction_t*)e)->transaction = transaction;
    ((uxe_on_transaction_t*)e)->portfolio = portfolio;
    ux_dispatch_event(portfolio->ctx, e, queued);
    if (portfolio->parent && portfolio->update_parent)
        emit_on_transaction(portfolio->parent, transaction, queued);
    //TODO:
    //portfolioStatistics.OnTransaction(transaction);
}

void ux_portfolio_on_execution_report(ux_portfolio_t *portfolio, uxe_execution_report_t *report, int queued)
{
    ux_transaction_t *transaction = NULL;
    switch(report->exec_type) {
    case UX_EXEC_REJECTED:
    case UX_EXEC_EXPIRED:
    case UX_EXEC_CANCELLED:
        transaction = ux_portfolio_get_transaction_by_order_id(portfolio, report->order->id);
        if (transaction) {
            transaction->is_done = TRUE;
            kv_set(&portfolio->transaction_by_order_id, report->order->id, 0);
        }
        break;
    case UX_EXEC_TRADE:
        transaction = ux_portfolio_get_transaction_by_order_id(portfolio, report->order->id);
        if (!transaction) {
            transaction = ux_zalloc(sizeof(ux_transaction_t));
            ux_portfolio_add_transaction(portfolio, transaction, report->order->id, queued);
        }
        ux_fill_t *fill = ux_zalloc(sizeof(ux_fill_t));
        ux_transaction_add_fill(transaction, fill);
        ux_portfolio_add_fill(portfolio, fill, queued);
        if (report->order_status == UX_ORDER_STATUS_FILLED) {
            transaction->is_done = TRUE;
            kv_set(&portfolio->transaction_by_order_id, report->order->id, 0);
            emit_on_transaction(portfolio, transaction, queued);
        }
        break;
    default:
        return;
    }
}
