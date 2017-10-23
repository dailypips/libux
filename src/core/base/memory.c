/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include <stdlib.h> /* malloc */
#include <string.h> /* memset */

#if defined(_WIN32)
#include <malloc.h> /* malloc */
#endif

#include <assert.h>
#include <errno.h>
#include <ux/base/ux_memory.h>

typedef struct {
    ux_malloc_func local_malloc;
    ux_realloc_func local_realloc;
    ux_calloc_func local_calloc;
    ux_free_func local_free;
} ux__allocator_t;

static ux__allocator_t ux__allocator = {
    malloc,
    realloc,
    calloc,
    free,
};

char* ux_strdup(const char* s)
{
    assert(s != NULL);
    size_t len = strlen(s) + 1;
    char* m = ux_malloc(len);
    if (m == NULL)
        return NULL;
    return memcpy(m, s, len);
}

char* ux_strndup(const char* s, size_t n)
{
    assert(s != NULL);
    char* m;
    size_t len = strlen(s);
    if (n < len)
        len = n;
    m = ux_malloc(len + 1);
    if (m == NULL)
        return NULL;
    m[len] = '\0';
    return memcpy(m, s, len);
}

void* ux_malloc(size_t size)
{
    assert(size > 0);
    void *p =  ux__allocator.local_malloc(size);
    if (!p)
        abort();
    return p;
}

void ux_free(void* ptr)
{
    assert(ptr != NULL);
    int saved_errno;

    saved_errno = errno;
    ux__allocator.local_free(ptr);
    errno = saved_errno;
}

void* ux_calloc(size_t count, size_t size)
{
    assert(count * size > 0);
    return ux__allocator.local_calloc(count, size);
}

void* ux_realloc(void* ptr, size_t size)
{
    assert(ptr != NULL);
    assert(size > 0);
    return ux__allocator.local_realloc(ptr, size);
}

void* ux_zalloc(size_t size)
{
    assert(size > 0);
    void* p = ux_malloc(size);
    if (p)
        memset(p, '\0', size);
    return p;
}

int ux_replace_allocator(ux_malloc_func malloc_func,
    ux_realloc_func realloc_func,
    ux_calloc_func calloc_func,
    ux_free_func free_func)
{
    if (malloc_func == NULL || realloc_func == NULL || calloc_func == NULL || free_func == NULL) {
        return -1;
    }

    ux__allocator.local_malloc = malloc_func;
    ux__allocator.local_realloc = realloc_func;
    ux__allocator.local_calloc = calloc_func;
    ux__allocator.local_free = free_func;

    return 0;
}
