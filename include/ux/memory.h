/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_MEMORY_H__
#define __UX_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ux/common.h>

/* memory module */
typedef void* (*ux_malloc_func)(size_t size);
typedef void* (*ux_realloc_func)(void* ptr, size_t size);
typedef void* (*ux_calloc_func)(size_t count, size_t size);
typedef void (*ux_free_func)(void* ptr);

UX_EXTERN int ux_replace_allocator(ux_malloc_func malloc_func,
                                   ux_realloc_func realloc_func,
                                   ux_calloc_func calloc_func,
                                   ux_free_func free_func);

UX_EXTERN void* ux_malloc(size_t size);
UX_EXTERN void* ux_calloc(size_t count, size_t size);
UX_EXTERN void* ux_zalloc(size_t size);
UX_EXTERN void  ux_free(void *ptr);
UX_EXTERN void* ux_realloc(void *ptr, size_t size);
UX_EXTERN void* ux_malloc_aligned(size_t size, size_t alignment);
UX_EXTERN char *ux_strdup(const char *src);
UX_EXTERN char* ux_strndup(const char* s, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __UX_MEMORY_H__
