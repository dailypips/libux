/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_ID_ARRAY_H__
#define __UX_ID_ARRAY_H__

#include <ux/base/ux_common.h>
#include <ux/base/ux_memory.h>
#include <string.h>

typedef struct ux_idarray_s {
    size_t n;
    size_t m;
    uintptr_t* a;
} ux_idarray_t;

#define roundup32(x) (--(x), (x) |= (x) >> 1, (x) |= (x) >> 2, (x) |= (x) >> 4, (x) |= (x) >> 8, (x) |= (x) >> 16, ++(x))

static inline void kv_init(ux_idarray_t* v)
{
    v->n = v->m = 0;
    v->a = NULL;
}

static inline void kv_destroy(ux_idarray_t* v)
{
    ux_free(v->a);
}

static inline uintptr_t kv_get(ux_idarray_t* v, size_t index)
{
    if(UX_UNLIKELY(index > v->n))
        return 0;
    return v->a[index];
}

static inline void kv_set(ux_idarray_t* v, size_t i, uintptr_t val)
{
    if (UX_UNLIKELY(v->m <= i)) {
        v->m = i + 1;
        roundup32(v->m);
        v->a = (uintptr_t*)ux_realloc(v->a, sizeof(uintptr_t) * v->m);
        memset(v->a + i, '\0', v->m - i);
    }
    v->n = i + 1;
    v->a[i] = val;
}

static inline void kv_clear(ux_idarray_t *v)
{
    kv_destroy(v);
    kv_init(v);
}


#endif // __UX_ID_ARRAY_H__
