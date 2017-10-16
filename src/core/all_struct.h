#include <ux/event_def.h>

/* -E -P */
/* gsed -e 's/__CR__/\n/g' -e 's/__TAB__/\t/g' */

#ifndef __ux_define__
#define __ux_define__ #define
#endif

#define EVENT_FIELD(name, lname, destory, clone, processor)	__ux_define__ UXE_##name##_FIELDS __CR__

EVENTDEF(EVENT_FIELD)
#undef EVENT_FIELD

#define EVENT_STRUCT(name, lname, destory, clone, processor)	struct uxe_##lname##_s { __CR__ \
    __TAB__ UXE_COMMON_FIELDS __CR__ \
    __TAB__ UXE_##name##_FIELDS __CR__\
    }; __CR__ __CR__

EVENTDEF(EVENT_STRUCT)
#undef EVENTE_STRUCT

