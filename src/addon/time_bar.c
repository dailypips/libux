#include <ux/base/ux_common.h>
#include "queue.h"
#include "time_bar.h"
#include "dispatch.h"

static void emit_bar_open(ux_ctx_t *ctx, ux_bar_generator_t* generator)
{
    generator->bar->status = UX_BAR_STATUS_OPEN;
    ux_dispatch_event(ctx, (ux_event_t*)generator->bar, UX_DISPATCH_IMMEDIATELY);
}

static void emit_bar(ux_ctx_t *ctx, ux_bar_generator_t* generator)
{
    generator->bar->status = UX_BAR_STATUS_CLOSE;
    ux_dispatch_event(ctx, (ux_event_t*)generator->bar, UX_DISPATCH_IMMEDIATELY);
    generator->bar = NULL;
}

static void time_bar_on_reminder(uxe_reminder_t* r)
{
    time_bar_generator_t* item = (time_bar_generator_t*)(r->user_data);

    if (item->clock_type == UX_CLOCK_LOCAL)
        item->bar->timestamp = r->timeout;
    else
        item->bar->timestamp = bus_get_exchange_time(item->ctx);

    emit_bar(item->ctx, (ux_bar_generator_t*)item);
}

static ux_time_t round_start_time(time_bar_generator_t* generator, ux_event_tick_t* tick)
{
    /* bar_size 表达秒数 */
    int bar_size = generator->bar_size;
    return tick->timestamp;
}

static void time_bar_on_tick(ux_bar_generator_t* node, ux_event_tick_t* tick)
{
    uxe_bar_t* bar;
    time_bar_generator_t* item = (time_bar_generator_t*)node;

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
            emit_bar_open(item->ctx, (ux_bar_generator_t*)item);
        }
    } else {
        bar = (uxe_bar_t*)ux_event_zalloc(UXE_BAR);
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
            emit_bar_open(item->ctx, (ux_bar_generator_t*)item);
        } else {
            item->start_time = round_start_time(item, tick);

            uxe_reminder_t* r = (uxe_reminder_t*)ux_event_zalloc(UXE_REMINDER);
            r->clock_type = item->clock_type;
            r->repeat = item->bar_size * TICKS_PER_SECOND;
            r->timeout = item->start_time + r->repeat;
            r->callback = time_bar_on_reminder;
            r->user_data = item;

            ux_bar_factory_add_reminder(item->ctx, r);
        }
    }
    if (UX_LIKELY(item->started))
        item->bar->tick_count++;
}

void time_bar_generator_init(time_bar_generator_t* generator, long bar_size)
{
    QUEUE_INIT(&generator->queue_node);
    generator->clock_type = UX_CLOCK_LOCAL;
    generator->on_tick = time_bar_on_tick;
    generator->start_time = MIN_DATE_TIME;
    generator->started = 0;
    generator->bar_type = UX_BAR_TYPE_TIME;
    generator->bar_size = bar_size;
}

void time_bar_generator_destroy(time_bar_generator_t* generator)
{
    if(generator->bar) {
        ux_event_unref((ux_event_t*)generator->bar);
    }
}
