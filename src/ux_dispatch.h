#ifndef __UX_EVENT_DISPATCH_H__
#define __UX_EVENT_DISPATCH_H__


#include "ux_event.h"
#include "ux_common.h"
#include "ux_loop.h"

UX_FUNC void ux_event_default_dispatch(ux_loop_t *loop, ux_event_t *e);

UX_FUNC void ux_event_reminder_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void ux_event_ask_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void ux_event_bid_dispatch(ux_loop_t *loop, ux_event_t *e);
UX_FUNC void ux_event_trade_dispatch(ux_loop_t *loop, ux_event_t *e);


#endif // __UX_EVENT_DISPATCH_H__

