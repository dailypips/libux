/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_ID_ARRAY_H__
#define __UX_ID_ARRAY_H__

#include <ux/base/ux_memory.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/* fit for inc count from zero */
typedef struct ux_idarray_s {
    size_t m;
    uintptr_t* a;
} ux_idarray_t;

#define UX_IDARRAY_MIN_SIZE 16
#define UX_IDARRAY_MAX_SIZE INT_MAX /* don't set to UINT_MAX */

#define roundup32(x) (--(x), (x) |= (x) >> 1, (x) |= (x) >> 2, (x) |= (x) >> 4, (x) |= (x) >> 8, (x) |= (x) >> 16, ++(x))

static inline void ida_int(ux_idarray_t* v)
{
    v->m = 0;
    v->a = NULL;
}

static inline void ida_destroy(ux_idarray_t* v)
{
    ux_free(v->a);
}

static inline uintptr_t ida_get(ux_idarray_t* v, size_t i)
{
    assert(i < UX_IDARRAY_MAX_SIZE);
    if(UX_UNLIKELY(i >= v->m))
        return 0;
    return v->a[i];
}

static inline void ida_set(ux_idarray_t* v, size_t i, uintptr_t val)
{
    assert(i < UX_IDARRAY_MAX_SIZE);
    if (UX_UNLIKELY(v->m <= i)) {
        v->m = i + 1;
        v->m = v->m < UX_IDARRAY_MIN_SIZE ? UX_IDARRAY_MIN_SIZE : v->m;
        roundup32(v->m);
        v->a = (uintptr_t*)ux_realloc(v->a, sizeof(uintptr_t) * v->m);
        memset(v->a + i, '\0', (v->m - i) * sizeof(uintptr_t));
    }
    v->a[i] = val;
}

static inline void ida_clear(ux_idarray_t *v)
{
    ida_destroy(v);
    ida_int(v);
}

#undef roundup32

#ifdef __cplusplus
}
#endif

#endif // __UX_ID_ARRAY_H__
