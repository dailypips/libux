#include "ux_internal.h"

static void loop_add_queue(ux_loop_t *loop, void *q)
{
    bus_add_queue(loop, (ux_queue_t*)q);
}

void ux_loop_add_queue(ux_loop_t* loop, ux_queue_t *q)
{
    ux_async_post(loop, loop_add_queue, q);
}

static inline void timewait(ux_loop_t* loop, int64_t timeout)
{
    if(timeout < 0) return;
    uv_mutex_lock(&loop->wait_mutex);
    if (timeout > 0)
        uv_cond_timedwait(&loop->wait_cond, &loop->wait_mutex, timeout);
    else
        uv_cond_wait(&loop->wait_cond, &loop->wait_mutex);
    uv_mutex_unlock(&loop->wait_mutex);
}


void ux_loop_wakeup(ux_loop_t* loop)
{
    uv_cond_signal(&loop->wait_cond);
}

void ux_async_post(ux_loop_t *loop, ux_async_cb async_cb, void *data)
{
    ux_async_t *async = ux_malloc(sizeof(ux_async_t));
    async->async_cb = async_cb;
    async->data = data;

    mpscq_push(&loop->async_queue, (mpscq_node*)&async->next);
    ux_loop_wakeup(loop);
}

static void loop_dispatch_event(ux_loop_t *loop, ux_event_t *e)
{
    event_dispatch dispatcher = g_eventclassinfo[e->type].dispatch;
    if(dispatcher)
        dispatcher(loop, e);
}

void ux_loop_run(ux_loop_t* loop, ux_run_mode mode)
{
    while (loop->stop_flag == 0) {
        /* step 1: call all async cb */
        mpscq_node* node;
        while ((node = mpscq_pop(&loop->async_queue))) {
            ux_async_t* async_node = container_of(node, ux_async_t, next);
            async_node->async_cb(loop, async_node->data);
        }
        /* step 2: pop bus */
        ux_event_t* e = bus_next_event(loop);

        if (e != NULL) {
            loop_dispatch_event(loop, e);
            if (mode == UX_RUN_ONCE || mode == UX_RUN_NOWAIT)
                break;
        } else {
            if (mode == UX_RUN_NOWAIT)
                break;

            int64_t timeout = bus_next_timeout(loop);
            timewait(loop, timeout);
        }
    }
}

void ux_loop_stop(ux_loop_t *loop)
{
    loop->stop_flag = 1;
}

void ux_loop_init(ux_loop_t *loop)
{
    uv_mutex_init(&loop->wait_mutex);
    uv_cond_init(&loop->wait_cond);
    loop->stop_flag = 0;
    mpscq_init(&loop->async_queue);

   bus_init(loop, UX_BUS_SIMULATION);
}

void ux_loop_destory(ux_loop_t *loop)
{
    uv_cond_destroy(&loop->wait_cond);
    uv_mutex_destroy(&loop->wait_mutex);
}
