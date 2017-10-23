/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "dispatch.h"
#include "context.h"
#include "event_vtable.h"

/*void on_event(ux_ctx_t *ctx, ux_event_t *e)
{
    if(event_manager_status == PAUSED && stepping &&
            (stepAtTypeId == 0 || stepStopAtTypeId == e.TypeId))
    {
        stepping = false;
    }

    ctx->event_count++;

    if (!enabled[e.TypeId])
        return;

    if (event_filter != NULL) {
        if (event_filter->filter(e) == NULL)
            return;
    }

    if (e.TypeId > 250)
        forward_event_to_strategy(e);

    if (event_handles[e.TypeId] != NULL) {
        event_handles[e.TypeId](e);
    }


    if(ctx->event_dispatcher != NULL)
        event_dispatcher(e);

    if(ctx->event_logger != NULL)
        event_logger(e);
}*/

static void on_event(ux_ctx_t *ctx, ux_event_t* e)
{
    // step 1: filter event

    // step 2: logger event

    // step 3: process event
    event_dispatch dispatcher = g_event_vtable[e->type].dispatch;
    if (dispatcher)
        dispatcher(ctx, e);

    // step4: dispatch to other

    // step5: unref
    ux_event_unref(e);
}

void ux_dispatch_event(ux_ctx_t *ctx, ux_event_t* e, ux_dispatch_mode mode)
{
    if (mode == UX_DISPATCH_IMMEDIATELY)
        on_event(ctx, e);
    else
        spscq_push(&ctx->buffed_event_queue, e);
}

void ux_emit_bufferd_events(ux_ctx_t *ctx)
{
    ux_event_t * e;
    while((e = spscq_pop(&ctx->buffed_event_queue))) {
        on_event(ctx, e);
    }
}

/* default process event */





