#ifndef __TIME_BAR_H__
#define __TIME_BAR_H__

#include <ux/bus/ux_bar_generator.h>

typedef struct time_bar_item_s {
    UX_BAR_GENERATOR_PUBLIC_FIELDS
    ux_bar_type bar_type;
    long bar_size;
    ux_clock_type clock_type;
    ux_time_t start_time;
    int started;
}time_bar_generator_t;

void time_bar_generator_init(time_bar_generator_t *generator, long bar_size);
void time_bar_generator_destroy(time_bar_generator_t *generator);


#endif // __TIME_BAR_H__
