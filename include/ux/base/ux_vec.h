/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_VEC_H__
#define __UX_VEC_H__

#include <ux/base/ux_memory.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/* fit for inc count from zero */
typedef struct ux_vec_s {
    size_t m;
    size_t n;
    uintptr_t *a;
} ux_vec_t;

#ifndef UX_VEC_MIN_SIZE
#define UX_VEC_MIN_SIZE 16
#endif

#ifndef UX_VEC_MAX_SIZE
#define UX_VEC_MAX_SIZE INT_MAX /* don't set to UINT_MAX */
#endif

#define roundup32(x) (--(x), (x) |= (x) >> 1, (x) |= (x) >> 2, (x) |= (x) >> 4, (x) |= (x) >> 8, (x) |= (x) >> 16, ++(x))

static inline void vec_init(ux_vec_t* v)
{
    v->m = v->n = 0;
    v->a = NULL;
}

static inline void vec_destroy(ux_vec_t* v)
{
    ux_free(v->a);
}

static inline uintptr_t vec_get(ux_vec_t* v, size_t i)
{
    assert(i < UX_VEC_MAX_SIZE);
    if(UX_UNLIKELY(i >= v->m))
        return 0;
    return v->a[i];
}

static inline void vec_add(ux_vec_t* v, uintptr_t val)
{
    if (v->n == v->m) {
        v->m = v->m ? v->m << 1 : UX_VEC_MIN_SIZE;
        v->a = ux_realloc(v->a, sizeof(uintptr_t) * v->m);
    }
    v->a[v->n++] = val;
}

static inline void vec_remove_at(ux_vec_t *v, size_t i)
{
    assert(i < v->n);
    memcpy(v->a + i, v->a + i + 1, sizeof(uintptr_t) * (v->n - i));
    v->n--;
}

static inline void vec_remove(ux_vec_t *v, uintptr_t val)
{
    for (size_t i = 0; i < v->n; i++) {
        if (v->a[i] == val) {
            vec_remove_at(v, i);
            break;
        }
    }
}

#define VEC_ADD(v,t) vec_add((v), (uintptr_t)(t))

#undef roundup32

#ifdef __cplusplus
}
#endif

#endif // __UX_VEC_H__
