/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_internal.h"

#define free_if(x) \
    if (x)         \
        ux_free(x);

static void event_news_destory(ux_event_t* e)
{
    ux_event_news_t* news = (ux_event_news_t*)e;
    free_if(news->urgency);
    free_if(news->headline);
    free_if(news->url);
    free_if(news->text);
}

static ux_event_t* event_news_clone(ux_event_t* event)
{
    assert(event->type == UX_EVENT_NEWS);
    ux_event_news_t* e = (ux_event_news_t*)event;
    ux_event_news_t* news = (ux_event_news_t*)ux_event_malloc(UX_EVENT_NEWS);

    news->provider = e->provider;
    news->instrument = e->instrument;
    news->urgency = ux_strdup(e->urgency);
    news->url = ux_strdup(e->url);
    news->headline = ux_strdup(e->headline);
    news->text = ux_strdup(e->text);

    return (ux_event_t*)news;
}

static void event_exception_destory(ux_event_t* e)
{
    ux_event_exception_t* exception = (ux_event_exception_t*)e;
    free_if(exception->source);
}

static ux_event_t* event_exception_clone(ux_event_t* e)
{
    assert(e->type == UX_EVENT_EXCEPTION);
    ux_event_exception_t* exception = (ux_event_exception_t*)e;
    ux_event_exception_t* item = (ux_event_exception_t*)ux_event_malloc(UX_EVENT_EXCEPTION);
    item->source = ux_strdup(exception->source);
    return (ux_event_t*)item;
}

static ux_event_t* event_default_clone(ux_event_t* e)
{
    UX_ASSERT(e->type < UX_EVENT_LAST);
    ux_event_t* result = ux_event_malloc(e->type);
    memcpy(result, e, g_eventclassinfo[e->type].size);
    ux_atomic_rel_store(&e->refcount, 1);
    return result;
}

static void event_l2snapshot_destory(ux_event_t* e)
{
    assert(e->type == UX_EVENT_L2SNAPSHOT);
    ux_event_l2snapshot_t* snapshot = (ux_event_l2snapshot_t*)e;
    free_if(snapshot->ticks);
}

static ux_event_t* event_l2snapshot_clone(ux_event_t* e)
{
    ux_event_l2snapshot_t* snapshot = (ux_event_l2snapshot_t*)event_default_clone(e);
    size_t size = (snapshot->num_bids + snapshot->num_asks) * sizeof(ux_tick_info_t);
    ux_tick_info_t* info = ux_malloc(size);
    memcpy(info, snapshot->ticks, size);
    snapshot->ticks = info;
    return (ux_event_t*)snapshot;
}

static void event_l2update_destory(ux_event_t* e)
{
    assert(e->type == UX_EVENT_L2UPDATE);
    ux_event_l2update_t* update = (ux_event_l2update_t*)e;
    free_if(update->entries);
}

static ux_event_t* event_l2update_clone(ux_event_t* e)
{
    ux_event_l2update_t* update = (ux_event_l2update_t*)event_default_clone(e);
    size_t size = (update->num_entries) * sizeof(ux_level2_t);
    ux_level2_t* info = ux_malloc(size);
    memcpy(info, update->entries, size);
    update->entries = info;
    return (ux_event_t*)update;
}

#define event_default_destory NULL
#define event_default_dispatch NULL

ux_event_class_info_t g_eventclassinfo[UX_EVENT_LAST] = {
#define CLASSENUM(euname, elname, edestory, eclone, edispatch)   \
    {                                                            \
        .size = sizeof(ux_event_##elname##_t),                   \
        .destory = (event_destory)event_##edestory##_destory,    \
        .clone = (event_clone)event_##eclone##_clone,            \
        .dispatch = (event_dispatch)event_##edispatch##_dispatch \
    },
    EVENTDEF(CLASSENUM)
#undef CLASSENUM
};
