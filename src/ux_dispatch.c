#include "ux_internal.h"

void ux_event_default_dispatch(ux_loop_t *loop, ux_event_t *e)
{

}

void ux_event_reminder_dispatch(ux_loop_t *loop, ux_event_t *e)
{

}

void ux_event_ask_dispatch(ux_loop_t *loop, ux_event_t *e)
{
    printf("ask ");
}

void ux_event_bid_dispatch(ux_loop_t *loop, ux_event_t *e)
{
     printf("bid ");
}

void ux_event_trade_dispatch(ux_loop_t *loop, ux_event_t *e)
{
     printf("trade ");
}
