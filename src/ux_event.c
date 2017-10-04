/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include <string.h>
#include "ux_event.h"
#include <stdarg.h>
#include <assert.h>
#include "ux_datetime.h"
#include "ux_mem.h"
#include "ux_dispatch.h"

/* event class info */
typedef void (*event_init)(ux_event_t* e, va_list args);
typedef void (*event_destory)(ux_event_t* e);
typedef ux_event_t* (*event_clone)(ux_event_t* e);
typedef void (*event_dispatch)(ux_loop_t *loop, ux_event_t* e);

typedef struct _ux_event_class_info_t {
    ux_event_flag flag;
    size_t size;
    event_init init;
    event_destory destory;
    event_clone clone;
    event_dispatch dispatch;
} ux_event_class_info_t;

/*
ux_event_t *event_reminder_init(ux_event_t *e, ux_clock_type ctype, reminder_cb callback, void *data)
{
    ux_event_reminder_t *revent = (ux_event_reminder_t *)e;

    revent->clock_type = ctype;
    revent->callback = callback;
    revent->user_data = data;

    return e;
}

ux_event_t *event_execption_init(ux_event_t *e, char* source)
{
    ux_event_on_exception_t *event = (ux_event_on_exception_t*)e;
    event->source = ux_strdup(source);
    return e;
}

ux_event_t *event_on_simulator_start_init(ux_event_t *e, datetime_t time1, datetime_t time2, long long count)
{
    ux_event_on_simulator_start_t * event = (ux_event_on_simulator_start_t*)e;
    event->time1 = time1;
    event->time2 = time2;
    event->count = count;

    return e;
}


ux_event_t * event_tick_init(ux_event_t *e, int provider, int instrument, datetime_t exchange_timestamp, double price, long size)
{
    ux_event_tick_t * tick = (ux_event_tick_t *)e;
    tick->exchange_timestamp = exchange_timestamp;
    tick->instrument = instrument;
    tick->provider = provider;
    tick->price = price;
    tick->size = size;
    return e;
}

ux_event_t* event_fundamental_init(ux_event_t *e, int provider, int instrument)
{
    ux_event_fundamental_t *fundamental = (ux_event_fundamental_t *)e;
    fundamental->provider = provider;
    fundamental->instrument = instrument;
    return e;
}

ux_event_t* event_queue_init(ux_event_t *e, void *q)
{
    ux_event_queue_t *queue = (ux_event_queue_t*)e;
    queue->timestamp = MIN_DATE_TIME;
    queue->q = q;
    return e;
}
*/

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

#define free_if(x) if(x) ux_free(x);
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

static ux_event_class_info_t _eventclassinfo[UX_EVENT_LAST] = {
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
static inline int event_check_type(ux_event_type type)
{
    if (type >= UX_EVENT_LAST)
        return 1;
    return 0;
}

static void
event_free(ux_event_t *e)
{
    event_destory destory = _eventclassinfo[e->type].destory;
    if (destory)
        destory(e);
    ux_free(e);
}

ux_event_t *ux_event_clone(ux_event_t *e)
{
    ux_event_t *result;
    event_clone clone = _eventclassinfo[e->type].clone;
    if(clone)
        result = clone(e);
    else {
        size_t size = _eventclassinfo[e->type].size;
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
    assert(!event_check_type(type) == 0);
    ux_event_t* e = ux_zalloc(_eventclassinfo[type].size);
    if (e) {
        e->type = type;
        e->flag = _eventclassinfo[type].flag;
        ux_atomic_rel_store(&e->refcount, 1);
    }
    return e;
}

void ux_event_dispatch(void *loop, ux_event_t *e)
{
    // TODO: add plugin extesion pointer: before_event_dispatch & after_event_dispatch
    event_dispatch dispatch = _eventclassinfo[e->type].dispatch;
    if (dispatch != NULL)
        dispatch(loop, e);
}
