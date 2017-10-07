#include "ux_internal.h"

void ux_wakeup(ux_loop_t* loop)
{
    uv_cond_signal(&loop->wait_cond);
}

void ux_async_post(ux_loop_t* loop, ux_async_cb async_cb, void* data)
{
    ux_async_t* async = ux_malloc(sizeof(ux_async_t));
    async->async_cb = async_cb;
    async->data = data;

    mpscq_push(&loop->async_queue, (mpscq_node*)&async->next);
    ux_wakeup(loop);
}

static int64_t bus_next_timeout(ux_loop_t* loop)
{
    ux_event_reminder_t* r = bus_timer_peek(loop, UX_CLOCK_LOCAL);

    if (r)
        return r->stop - ux_time_now();

    return 0;
}

static inline void timewait(ux_loop_t* loop, int64_t timeout)
{
    uv_mutex_lock(&loop->wait_mutex);
    if (timeout > 0)
        uv_cond_timedwait(&loop->wait_cond, &loop->wait_mutex, timeout);
    else
        uv_cond_wait(&loop->wait_cond, &loop->wait_mutex);
    uv_mutex_unlock(&loop->wait_mutex);
}

void ux_run(ux_loop_t* loop, ux_run_mode mode)
{
    while (loop->stop_flag == 0) {
        /* step 1: call all async cb */
        mpscq_node* node;
        while ((node = mpscq_pop(&loop->async_queue))) {
            ux_async_t* async_node = container_of(node, ux_async_t, next);
            async_node->async_cb(loop, async_node->data);
        }
        /* step 2: pop bus */
        ux_event_t* e = bus_dequeue(loop);

        if (e)
            dispatch_event(loop, e);

        if (mode == UX_RUN_NOWAIT || (mode == UX_RUN_ONCE && e))
            break;

        int64_t timeout = bus_next_timeout(loop);
        if (timeout > 0)
            timewait(loop, timeout);
    }
}

void ux_stop(ux_loop_t* loop)
{
    loop->stop_flag = 1;
}

ux_loop_t* ux_loop_new(void)
{
    ux_loop_t* loop = ux_zalloc(sizeof(ux_loop_t));
    ux_loop_init(loop);
    return loop;
}

void ux_loop_free(ux_loop_t* loop)
{
    ux_loop_destory(loop);
    ux_free(loop);
}
