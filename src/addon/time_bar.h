#ifndef __TIME_BAR_H__
#define __TIME_BAR_H__

#include <ux/ux.h>

typedef struct time_bar_item_s {
    UX_BAR_ITEM_PUBLIC_FIELDS
    //UX_BAR_ITEM_COMMON_FILEDS
    ux_clock_type clock_type;
    ux_time_t start_time;
    int started;
}time_bar_item_t;

void time_bar_item_init(time_bar_item_t *item);
void time_bar_item_destroy(time_bar_item_t *item);


#endif // __TIME_BAR_H__
