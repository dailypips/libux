/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_queue.h"
#include "ux_atomic.h"
#include "ux_event.h"
#include "ux_mem.h"
#include "ux_spscq.h"

ux_queue_t* ux_queue_init(ux_queue_t* q, unsigned int size, ux_queue_category category)
{
    int ret = spscq_init(&q->spsc, size);
    if (ret < 0) {
        return NULL;
    }
    q->bus = NULL;
    q->on_event = NULL;
    q->category = category;
    return q;
}

void ux_queue_destory(ux_queue_t* q)
{
    ux_event_t* e;
    while ((e = ux_queue_pop(q)))
        ux_event_unref(e);

    spscq_destroy(&q->spsc);
}

static void ux_queue_free(ux_queue_t* q)
{
    ux_queue_destory(q);
    ux_free(q);
}

void ux_queue_ref(ux_queue_t *q)
{
  assert(q != NULL);
  ux_atomic_full_fetch_add(&q->refcount, 1);
}

void ux_queue_unref(ux_queue_t *q)
{
   assert(q != NULL);
   if(ux_atomic_full_fetch_add(&q->refcount, -1) == 1)
       ux_queue_free(q);
}

/* 这里要改
 * 基本逻辑是这样的，
 * 如果向空队列压入事件，必须通知相应的runloop
 * 所以runloop必须是queue_t的成员
 * 通知runloop是异步的，把通知信号压入runloop的async_queue来实现
 * 如果要简化生命周期管理的话，queue_t必须是MPSC的节点子类型
 * pipe通过最小堆管理queue，所以queue_t必须是heap的节点子类型
 * 如果队列内容为空，pipe并不保存这个queue的指针，
 * queue的关闭是通过push QUEUE_CLOSE事件来完成
 */
/* only for producer */
int ux_queue_push(ux_queue_t* q, void* e)
{
    int is_empty = 0;
    if (spscq_is_empty(&q->spsc))
        is_empty = 1;

    ((ux_event_t*)e)->dummy = q;

    int ret = spscq_push(&q->spsc, e);

    if (is_empty && q->on_event) {
        UX_ASSERT(ret == 0);
        q->on_event(q);
    }

    return ret;
}

unsigned int ux_queue_size(ux_queue_t *q)
{
    return spscq_size(&q->spsc);
}

unsigned int ux_queue_capacity(ux_queue_t *q)
{
    return spscq_capacity(&q->spsc);
}

void *ux_queue_pop(ux_queue_t *q)
{
    return spscq_pop(&q->spsc);
}

void *ux_queue_peek(const ux_queue_t *q)
{
    return spscq_peek(&q->spsc);
}

int ux_queue_is_empty(ux_queue_t *q)
{
    return spscq_is_empty(&q->spsc);
}

int ux_queue_is_full(ux_queue_t *q)
{
    return spscq_is_full(&q->spsc);
}
