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

static ux_time_t round_time(time_bar_item_t* item, ux_event_tick_t *tick)
{
   /* bar_size 表达秒数 */
    return tick->timestamp;
}

void time_bar_on_tick(ux_barfactory_item_t* item, ux_event_tick_t* tick)
{
    if (item->bar) {
        if (tick->timestamp < item->bar->open_time)
            return;
        if (tick->price > item->bar->high)
            item->bar->high = tick->price;
        if (tick->price < item->bar->low)
            item->bar->low = tick->price;
        item->bar->close = tick->price;
        item->bar->volume += tick->size;
        item->bar->timestamp = tick->timestamp;
    } else {
        ux_event_bar_t *bar = (ux_event_bar_t*)ux_event_malloc(UX_EVENT_BAR);

        bar->provider = tick->provider;
        bar->instrument = tick->instrument;
        bar->bar_type = ((time_bar_item_t*)item)->bar_type;
        bar->size = ((time_bar_item_t*)item)->bar_size;
        bar->open_time = round_time((time_bar_item_t*)item, tick); //TODO: 考虑取整到分钟
        bar->timestamp = tick->timestamp;
        bar->open = tick->price;
        bar->high = tick->price;
        bar->low = tick->price;
        bar->close = tick->price;
        bar->volume = tick->size;

        item->bar = bar;
        ux_barfactory_emit_bar_open(item->factory, (ux_barfactory_item_t*)item);

        ux_event_reminder_t* r = (ux_event_reminder_t*)ux_event_malloc(UX_EVENT_REMINDER);
        r->clock_type = ((time_bar_item_t*)item)->clock_type;
        r->repeat = ((time_bar_item_t*)item)->bar_size * TICKS_PER_SECOND; // bar_size 表达为秒数
        r->timeout = bar->open_time + r->repeat;
        r->callback = time_bar_on_reminder;
        r->user_data = item;
        ux_barfactory_add_reminder(item->factory, r);
    }
    item->bar->tick_count++;
}

void time_bar_item_init(time_bar_item_t* item)
{
    ux_barfactory_item_init((ux_barfactory_item_t*)item);
    item->clock_type = UX_CLOCK_LOCAL;
    item->on_tick = time_bar_on_tick;
}

void time_bar_item_destory(time_bar_item_t* item)
{
    ux_barfactory_item_destory((ux_barfactory_item_t*)item);
}
