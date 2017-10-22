#include <uv.h>
#include <ux/bus/ux_context.h>
#include "context.h"
#include "mpscq.h"
#include "spscq.h"
#include "dispatch.h"


void ux_wakeup(ux_ctx_t *ctx)
{
    uv_cond_signal(&ctx->wait_cond);
}

void ux_async_post(ux_ctx_t *ctx, ux_async_cb async_cb, void* data)
{
    ux_async_t* async = ux_malloc(sizeof(ux_async_t));
    async->async_cb = async_cb;
    async->data = data;

    mpscq_push(&ctx->async_queue, (mpscq_node*)&async->next);
    ux_wakeup(ctx);
}

static int64_t bus_next_timeout(ux_ctx_t *ctx)
{
    uxe_reminder_t* r = bus_timer_peek(ctx, UX_CLOCK_LOCAL);

    if (r)
        return r->timeout - ux_time_now();

    return 0;
}

static inline void timewait(ux_ctx_t *ctx, int64_t timeout)
{
    uv_mutex_lock(&ctx->wait_mutex);
    if (timeout > 0)
        uv_cond_timedwait(&ctx->wait_cond, &ctx->wait_mutex, timeout);
    else
        uv_cond_wait(&ctx->wait_cond, &ctx->wait_mutex);
    uv_mutex_unlock(&ctx->wait_mutex);
}

void ux_run(ux_ctx_t *ctx, ux_run_mode mode)
{
    while (ctx->stop_flag == 0) {
        /* step 1: call all async cb */
        mpscq_node* node;
        while ((node = mpscq_pop(&ctx->async_queue))) {
            ux_async_t* async_node = container_of(node, ux_async_t, next);
            async_node->async_cb(ctx, async_node->data);
        }
        /* step 2: pop bus */
        ux_event_t* e = bus_dequeue(ctx);

        if (e)
            ux_dispatch_event(ctx, e, UX_DISPATCH_IMMEDIATELY);

        if (mode == UX_RUN_NOWAIT || (mode == UX_RUN_ONCE && e))
            break;

        int64_t timeout = bus_next_timeout(ctx);
        if (timeout > 0)
            timewait(ctx, timeout);
    }
}

void ux_stop(ux_ctx_t *ctx)
{
    ctx->stop_flag = 1;
}

ux_ctx_t* ux_ctx_new(void)
{
    ux_ctx_t *ctx = ux_zalloc(sizeof(ux_ctx_t));
    ux_ctx_init(ctx);
    return ctx;
}

void ux_ctx_free(ux_ctx_t *ctx)
{
    ux_ctx_destroy(ctx);
    ux_free(ctx);
}


void ux_ctx_init(ux_ctx_t *ctx)
{
    uv_mutex_init(&ctx->wait_mutex);
    uv_cond_init(&ctx->wait_cond);
    ctx->stop_flag = 0;
    mpscq_init(&ctx->async_queue);

   bus_init(ctx);
}

void ux_ctx_destroy(ux_ctx_t *ctx)
{
    bus_destroy(ctx);
    uv_cond_destroy(&ctx->wait_cond);
    uv_mutex_destroy(&ctx->wait_mutex);
    ux_ctx_clear(ctx);
}


void ux_ctx_clear(ux_ctx_t *ctx)
{
    bus_clear(ctx);
}
