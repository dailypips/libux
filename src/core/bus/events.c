/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "event_vtable.h"
#include <ux/event/ux_events.h>
#include "atomic.h"

#define free_if(x) \
    if (x)         \
        ux_free(x);

void event_news_destroy(ux_event_t* e)
{
    uxe_news_t* news = (uxe_news_t*)e;
    free_if(news->urgency);
    free_if(news->headline);
    free_if(news->url);
    free_if(news->text);
}

ux_event_t* event_news_clone(ux_event_t* event)
{
    assert(event->type == UXE_NEWS);
    uxe_news_t* e = (uxe_news_t*)event;
    uxe_news_t* news = (uxe_news_t*)ux_event_zalloc(UXE_NEWS);

    news->provider = e->provider;
    news->instrument = e->instrument;
    news->urgency = ux_strdup(e->urgency);
    news->url = ux_strdup(e->url);
    news->headline = ux_strdup(e->headline);
    news->text = ux_strdup(e->text);

    return (ux_event_t*)news;
}

void event_exception_destroy(ux_event_t* e)
{
    uxe_exception_t* exception = (uxe_exception_t*)e;
    free_if(exception->source);
}

ux_event_t* event_exception_clone(ux_event_t* e)
{
    assert(e->type == UXE_EXCEPTION);
    uxe_exception_t* exception = (uxe_exception_t*)e;
    uxe_exception_t* item = (uxe_exception_t*)ux_event_zalloc(UXE_EXCEPTION);
    item->source = ux_strdup(exception->source);
    return (ux_event_t*)item;
}

ux_event_t* event_default_clone(ux_event_t* e)
{
    UX_ASSERT(e->type < UXE_LAST);
    ux_event_t* result = ux_event_zalloc(e->type);
    memcpy(result, e, g_event_vtable[e->type].size);
    ux_atomic_rel_store(&e->refcount, 1);
    return result;
}

void event_level2_snapshot_destroy(ux_event_t* e)
{
    assert(e->type == UXE_LEVEL2_SNAPSHOT);
    uxe_level2_snapshot_t* snapshot = (uxe_level2_snapshot_t*)e;
    free_if(snapshot->bids);
    free_if(snapshot->asks);
}

ux_event_t* event_level2_snapshot_clone(ux_event_t* e)
{
    uxe_level2_snapshot_t* dst = (uxe_level2_snapshot_t*)event_default_clone(e);

    size_t bid_size = dst->num_bids * sizeof(ux_tick_info_t);
    size_t ask_size = dst->num_asks * sizeof(ux_tick_info_t);

    ux_tick_info_t* bids = ux_malloc(bid_size);
    ux_tick_info_t* asks = ux_malloc(ask_size);

    memcpy(bids, dst->bids, bid_size);
    memcpy(asks, dst->asks, ask_size);

    dst->bids = bids;
    dst->asks = asks;

    return (ux_event_t*)dst;
}

void event_level2_update_destroy(ux_event_t* e)
{
    assert(e->type == UXE_LEVEL2_UPDATE);
    uxe_level2_update_t* update = (uxe_level2_update_t*)e;
    free_if(update->entries);
}

ux_event_t* event_level2_update_clone(ux_event_t* e)
{
    uxe_level2_update_t* update = (uxe_level2_update_t*)event_default_clone(e);
    size_t size = (update->num_entries) * sizeof(ux_level2_t);
    ux_level2_t* info = ux_malloc(size);
    memcpy(info, update->entries, size);
    update->entries = info;
    return (ux_event_t*)update;
}
// TODO:
void event_account_report_destroy(ux_event_t *e)
{

}

ux_event_t* event_account_report_clone(ux_event_t *e)
{
    return e;
}

void event_execution_report_destroy(ux_event_t *e)
{

}

ux_event_t* event_execution_report_clone(ux_event_t *e)
{
    return e;
}
void event_execution_command_destroy(ux_event_t *e)
{

}

ux_event_t* event_execution_command_clone(ux_event_t *e)
{
    return e;
}

#define event_default_destroy NULL
#define event_default_dispatch NULL

uxe_class_vtable_t g_event_vtable[UXE_LAST] = {
#define CLASSENUM(euname, elname, edestroy, eclone, edispatch)   \
    {                                                            \
        .size = sizeof(uxe_##elname##_t),                   \
        .destroy = (event_destroy)event_##edestroy##_destroy,    \
        .clone = (event_clone)event_##eclone##_clone,            \
        .dispatch = (event_dispatch)event_##edispatch##_dispatch \
    },
    EVENTDEF(CLASSENUM)
#undef CLASSENUM
};
