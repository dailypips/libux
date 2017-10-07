/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include "ux_internal.h"

#define free_if(x) if(x) ux_free(x);

ux_event_t* event_news_init(ux_event_t *e, int provider, int instrument, char *urgency, char *url, char *headline, char *text)
{
    ux_event_news_t *news = (ux_event_news_t *)e;

    news->provider = provider;
    news->instrument = instrument;
    news->urgency = ux_strdup(urgency);
    news->headline = ux_strdup(headline);
    news->url = ux_strdup(url);
    news->text = ux_strdup(text);

    return e;
}


void event_news_destory(ux_event_t *e)
{
     ux_event_news_t *news = (ux_event_news_t *)e;
     free_if(news->urgency);
     free_if(news->headline);
     free_if(news->url);
     free_if(news->text);
}

ux_event_t* event_news_clone(ux_event_t *e)
{
    assert(e->type == UX_EVENT_NEWS);
    ux_event_news_t *news = (ux_event_news_t*)e;
    ux_event_news_t *item = (ux_event_news_t *)ux_event_malloc(UX_EVENT_NEWS);
    event_news_init((ux_event_t*)item, news->provider, news->instrument, news->urgency, news->url, news->headline, news->text);
    return (ux_event_t*)item;
}

ux_event_t* event_exception_init(ux_event_t *e, char *source)
{
    ux_event_exception_t *exception = (ux_event_exception_t *)e;

    exception->source = ux_strdup(source);

    return e;
}

void event_exception_destory(ux_event_t *e)
{
     ux_event_exception_t *exception = (ux_event_exception_t *)e;
     free_if(exception->source);
}

ux_event_t* event_exception_clone(ux_event_t *e)
{
    assert(e->type == UX_EVENT_EXCEPTION);
    ux_event_exception_t *exception = (ux_event_exception_t*)e;
    ux_event_exception_t *item = (ux_event_exception_t *)ux_event_malloc(UX_EVENT_EXCEPTION);
    event_exception_init((ux_event_t*)item, exception->source);
    return (ux_event_t*)item;
}

ux_event_class_info_t g_eventclassinfo[UX_EVENT_LAST] = {
#define CLASSENUM(ename, lename, einit, edestory, eclone, edispatch, eflag)	\
    {   \
        .init = (event_init)einit, \
        .destory = (event_destory)edestory, \
        .clone = (event_clone)eclone, \
        .flag = eflag, \
        .size = sizeof(ux_event_##lename##_t), \
        .dispatch = ux_event_##edispatch##_dispatch \
    },
EVENTDEF(CLASSENUM)
#undef CLASSENUM
};

/* public api */
size_t ux_event_size(ux_event_type type)
{
    return g_eventclassinfo[e->type].size;
}

static inline int event_check_type(ux_event_type type)
{
    if (type >= UX_EVENT_LAST)
        return 1;
    return 0;
}

static void
event_free(ux_event_t *e)
{
    event_destory destory = g_eventclassinfo[e->type].destory;
    if (destory)
        destory(e);
    ux_free(e);
}

ux_event_t *ux_event_clone(ux_event_t *e)
{
    ux_event_t *result;
    event_clone clone = g_eventclassinfo[e->type].clone;
    if(clone)
        result = clone(e);
    else {
        size_t size = g_eventclassinfo[e->type].size;
        result = ux_malloc(size);
        memcpy(result, e, size);
        ux_atomic_rel_store(&e->refcount, 1);
    }
    return result;
}

ux_event_t *ux_event_ref(ux_event_t *event)
{
  assert(event != NULL);
  ux_atomic_full_fetch_add(&event->refcount, 1);
  return event;
}

void ux_event_unref(ux_event_t *event)
{
   assert(event != NULL);
   if(ux_atomic_full_fetch_add(&event->refcount, -1) == 1)
       event_free(event);
}

ux_event_t* ux_event_malloc(ux_event_type type)
{
    assert(event_check_type(type) == 0);
    ux_event_t* e = ux_zalloc(g_eventclassinfo[type].size);
    if (e) {
        e->type = type;
        e->flag = g_eventclassinfo[type].flag;
        ux_atomic_rel_store(&e->refcount, 1);
    }
    return e;
}

void ux_event_dispatch(void *loop, ux_event_t *e)
{
    // TODO: add plugin extesion pointer: before_event_dispatch & after_event_dispatch
    event_dispatch dispatch = g_eventclassinfo[e->type].dispatch;
    if (dispatch != NULL)
        dispatch(loop, e);
}
