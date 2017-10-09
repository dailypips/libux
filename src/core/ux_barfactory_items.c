/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

static UX_AINLINE ux_loop_t* get_loop(ux_barfactory_t* factory)
{
    UX_ASSERT(factory != NULL);
    return container_of(factory, ux_loop_t, bar_factory);
}

static void time_bar_on_reminder(ux_event_reminder_t* r)
{
    time_bar_item_t* item = (time_bar_item_t*)(r->user_data);

    if (item->clock_type == UX_CLOCK_LOCAL)
        item->bar->timestamp = r->timeout;
    else
        item->bar->timestamp = bus_get_exchange_time(get_loop(item->factory));

    ux_barfactory_emit_bar(item->factory, (ux_barfactory_item_t*)item);
}

static ux_time_t round_start_time(time_bar_item_t* item, ux_event_tick_t* tick)
{
    /* bar_size 表达秒数 */
    int bar_size = item->bar_size;
    return tick->timestamp;
}

void time_bar_on_tick(ux_barfactory_item_t* node, ux_event_tick_t* tick)
{
    ux_event_bar_t* bar;
    time_bar_item_t* item = (time_bar_item_t*)node;

    if (item->bar) {

        if (!item->started && tick->timestamp < item->start_time) /*忽略开始时间之前的数据*/
            return;

        bar = item->bar;

        if (UX_LIKELY(item->started)) {
            if (tick->price > bar->high)
                bar->high = tick->price;
            if (tick->price < bar->low)
                bar->low = tick->price;
            bar->close = tick->price;
            bar->volume += tick->size;
            bar->timestamp = tick->timestamp;
        } else {
            item->started = 1;
            bar->timestamp = tick->timestamp;
            bar->open = tick->price;
            bar->high = tick->price;
            bar->low = tick->price;
            bar->close = tick->price;
            bar->volume = tick->size;
            ux_barfactory_emit_bar_open(item->factory, (ux_barfactory_item_t*)item);
        }
    } else {
        bar = (ux_event_bar_t*)ux_event_malloc(UX_EVENT_BAR);
        item->bar = bar;
        bar->provider = tick->provider;
        bar->instrument = tick->instrument;
        bar->bar_type = item->bar_type;
        bar->size = item->bar_size;

        if (UX_LIKELY(item->started)) {
            bar->open_time = tick->timestamp;
            bar->timestamp = tick->timestamp;
            bar->open = tick->price;
            bar->high = tick->price;
            bar->low = tick->price;
            bar->close = tick->price;
            bar->volume = tick->size;
            ux_barfactory_emit_bar_open(item->factory, (ux_barfactory_item_t*)item);
        } else {
            item->start_time = round_start_time(item, tick);

            ux_event_reminder_t* r = (ux_event_reminder_t*)ux_event_malloc(UX_EVENT_REMINDER);
            r->clock_type = item->clock_type;
            r->repeat = item->bar_size * TICKS_PER_SECOND;
            r->timeout = item->start_time + r->repeat;
            r->callback = time_bar_on_reminder;
            r->user_data = item;

            ux_barfactory_add_reminder(item->factory, r);
        }
    }
    if (UX_LIKELY(item->started))
        item->bar->tick_count++;
}

void time_bar_item_init(time_bar_item_t* item)
{
    ux_barfactory_item_init((ux_barfactory_item_t*)item);
    item->clock_type = UX_CLOCK_LOCAL;
    item->on_tick = time_bar_on_tick;
    item->start_time = MIN_DATE_TIME;
    item->started = 0;
}

void time_bar_item_destory(time_bar_item_t* item)
{
    ux_barfactory_item_destory((ux_barfactory_item_t*)item);
}
