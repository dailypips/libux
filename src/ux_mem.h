#ifndef __UX_MEM_H__
#define __UX_MEM_H__

#include <jemalloc/jemalloc.h>
#include <stdarg.h>
#include "ux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* using jemalloc standard api */
static UX_AINLINE void* ux_malloc(size_t size)
{
    return malloc(size);
}

static UX_AINLINE void* ux_zalloc(size_t size)
{
	void *p = ux_malloc(size);
	memset(p, '\0', size);
	return p;
}

static UX_AINLINE  void  ux_free(void *ptr)
{
	free(ptr);
}

static UX_AINLINE  void* ux_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

static UX_AINLINE void* ux_malloc_aligned(size_t size, size_t alignment)
{
       return aligned_alloc(alignment, size);
}

UX_EXPORT char *ux_strdup(const char *src);
UX_EXPORT int   ux_asprintf(char **strp, const char *format, ...) UX_PRINT_FOMAT_CHECK(2, 3);


#ifdef __cplusplus
}
#endif


#endif // __UX_MEM_H__
