/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "position.h"
#include "instrument.h"
#include "transaction.h"
#include "fill.h"
#include "math.h"
#include "queue.h"
#include "fill.h"
#include "portfolio.h"

static double get_value(ux_position_t *position, double val)
{
    if (position->instrument->factor != 0.0)
        return val * position->instrument->factor;
    return val;
}

static double calc_pnl(ux_position_t *position, double price, double qty)
{
    double val;
    if (position->side == UX_POSITION_SIDE_LONG)
        val = price - position->avg_price;
    else
        val = position->avg_price - price;
    return get_value(position, qty * val);
}

static void update_fill(ux_position_t *position, ux_fill_t *fill)
{
    if (position->amount == 0.0) {
        position->open_value = ux_fill_get_value(fill);
        position->avg_price = fill->price;
    }

    if ((position->side == UX_POSITION_SIDE_LONG && fill->side == UX_ORDER_SIDE_BUY) ||
         (position->side == UX_POSITION_SIDE_SHORT && fill->side == UX_ORDER_SIDE_SELL))
    {
        position->open_value += ux_fill_get_value(fill);
        if (position->instrument->factor != 0.0) {
            position->avg_price = position->open_value / (fabs(position->amount) + fill->qty) / position->instrument->factor;
        }else
            position->avg_price = position->open_value / (fabs(position->amount) + fill->qty);
    }else {
        if (fabs(position->amount) == fill->qty) {
            position->pnl += calc_pnl(position, fill->price, fill->qty);
            position->open_value = 0.0;
            position->avg_price = 0.0;
            return;
        }
        if(fabs(position->amount) > fill->qty)
        {
            position->pnl += calc_pnl(position, fill->price, fill->qty);
            position->open_value -= get_value(position, fill->qty * position->avg_price);
            return;
        }
        position->pnl += calc_pnl(position, fill->price, fabs(position->amount));
        double qty = fill->qty - fabs(position->amount);
        position->open_value = get_value(position, qty * fill->price);
        if (position->instrument->factor != 0.0)
            position->avg_price = position->open_value / qty / position->instrument->factor;
        else
            position->avg_price = position->open_value / qty;
    }
}

void ux_position_add_fill(ux_position_t *position, ux_fill_t *fill)
{
    QUEUE_INSERT_TAIL(&position->fills, &fill->queue_node);
    if (position->amount == 0.0)
        position->entry = fill;
    if (fill->side == UX_ORDER_SIDE_BUY)
        position->qty_bought += fill->qty;
    else
        position->qty_sold += fill->qty;
    update_fill(position, fill);
    position->amount = position->qty_bought - position->qty_sold;
}

double ux_position_get_value(ux_position_t *position)
{
    return position->portfolio->pricer->get_value(position->portfolio->pricer, position);
}

double ux_position_get_price(ux_position_t *position)
{
    return position->portfolio->pricer->get_price(position->portfolio->pricer, position);
}
