#ifndef __UX_MEM_H__
#define __UX_MEM_H__

#include <jemalloc/jemalloc.h>
#include <stdarg.h>
#include "ux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* using jemalloc standard api */
static inline void* ux_malloc(size_t size)
{
    return malloc(size);
}

static inline void* ux_zalloc(size_t size)
{
	void *p = ux_malloc(size);
	memset(p, '\0', size);
	return p;
}

static inline  void  ux_free(void *ptr)
{
	free(ptr);
}

static inline  void* ux_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

static inline void* ux_malloc_aligned(size_t size, size_t alignment)
{
       return aligned_alloc(alignment, size);
}

UX_API char *ux_strdup(const char *src);
UX_API int   ux_asprintf(char **strp, const char *format, ...) UX_PRINT_FOMAT_CHECK(2, 3);


#ifdef __cplusplus
}
#endif


#endif // __UX_MEM_H__
