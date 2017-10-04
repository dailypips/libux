#ifndef __UX_COMMON_H__
#define __UX_COMMON_H__

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define UX_ASSERT(x) assert((x))
#define UX_API
#define UX_PRINT_FOMAT_CHECK(x,y)

#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))


typedef uint64_t datetime_t;

typedef enum {
    UX_CATEGORY_MARKET = 0,
    UX_CATEGORY_EXECUTION,
    UX_CATEGORY_SERVICE,
    UX_CATEGORY_COMMAND,
    UX_CATEGORY_LAST
}ux_queue_category;

typedef enum {
    UX_CLOCK_LOCAL  = 0,
    UX_CLOCK_EXCHANGE,
    UX_CLOCK_LAST
}ux_clock_type;

#endif // __UX_COMMON_H__
