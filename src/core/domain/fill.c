#include "fill.h"

double ux_fill_get_value(ux_fill_t *fill)
{
    if (fill->instrument->factor != 0.0)
        return fill->price * fill->qty * fill->instrument->factor;
    return fill->price * fill->qty;
}
