/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef UX_API_H
#define UX_API_H

#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* base type */
typedef intptr_t ux_atomic_t;

typedef struct ux_list_s {
    void* queue[2];
}ux_list_t;

typedef struct ux_idarray_s {
    size_t n;
    size_t m;
    uintptr_t* a;
} ux_idarray_t;

typedef struct kh_int_s   ux_ihash_t;
typedef struct kh_str_s   ux_shash_t;
typedef struct kh_ilist_s ux_ihash_list_t;
typedef struct kh_slist_s ux_shash_list_t;

typedef int16_t ux_pid_t;   // provider id
typedef int32_t ux_id_t;    // order,instrument...
typedef int16_t ux_currency_t;




#ifdef __cplusplus
}
#endif

#endif // UX_API_H
