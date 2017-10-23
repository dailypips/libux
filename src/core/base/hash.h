/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __HASH_H__
#define __HASH_H__

#include "khash.h"
#include "queue.h"
#include <ux/base/ux_memory.h>

/* undefine the defaults */
#undef kcalloc
#undef kmalloc
#undef krealloc
#undef kfree

/* re-define, specifying alternate functions */
#define kcalloc(N, Z) ux_calloc(N, Z)
#define kmalloc(Z) ux_malloc(Z)
#define krealloc(P, Z) ux_realloc(P, Z)
#define kfree(P) ux_free(P)

typedef struct ux_list_s {
  void *queue[2];
} ux_list_t;

KHASH_DECLARE(int, khint32_t, void *)
KHASH_DECLARE(str, kh_cstr_t, void *)
KHASH_DECLARE(ilist, khint32_t, ux_list_t)
KHASH_DECLARE(slist, kh_cstr_t, ux_list_t)

typedef khash_t(int) ux_ihash_t;
typedef khash_t(str) ux_shash_t;
typedef khash_t(ilist) ux_list_ihash_t;
typedef khash_t(slist) ux_list_shash_t;

void *int_hash_get(ux_ihash_t *hash, khint32_t key);
void *str_hash_get(ux_shash_t *hash, const char *key);
void int_hash_set(ux_ihash_t *hash, khint32_t key, void *val);
void str_hash_set(ux_shash_t *hash, const char *key, void *val);
ux_list_t* int_hash_get_list(ux_list_ihash_t *hash, khint32_t key);
ux_list_t* str_hash_get_list(ux_list_shash_t *hash,  const char *key);
ux_list_t* int_hash_get_or_create_list(ux_list_ihash_t *hash, khint32_t key);
ux_list_t* str_hash_get_or_create_list(ux_list_shash_t *hash,  const char *key);

#endif // __HASH_H__
