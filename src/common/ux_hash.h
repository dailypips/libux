/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_HASH_H__
#define __UX_HASH_H__

#include <ux/memory.h>
#include "khash.h"
#include "queue.h"

/* undefine the defaults */
#undef kcalloc
#undef kmalloc
#undef krealloc
#undef kfree

/* re-define, specifying alternate functions */
#define kcalloc(N,Z)  ux_calloc(N,Z)
#define kmalloc(Z)    ux_malloc(Z)
#define krealloc(P,Z) ux_realloc(P,Z)
#define kfree(P)      ux_free(P)

typedef struct _list_s {
    void *queue[2];
}_list_t;

KHASH_DECLARE(int, khint32_t, void*)
KHASH_DECLARE(str, kh_cstr_t, void*)
KHASH_DECLARE(list, khint32_t, _list_t)

#endif // __UX_HASH_H__
