#include "hash.h"
#include <ux/base/ux_common.h>

__KHASH_IMPL(int, kh_inline klib_unused, khint32_t, void *, 1, kh_int_hash_func,
             kh_int_hash_equal)
__KHASH_IMPL(str, kh_inline klib_unused, kh_cstr_t, void *, 1, kh_str_hash_func,
             kh_str_hash_equal)
__KHASH_IMPL(ilist, kh_inline klib_unused, khint32_t, ux_list_t, 1,
             kh_int_hash_func, kh_int_hash_equal)
__KHASH_IMPL(slist, kh_inline klib_unused, kh_cstr_t, ux_list_t, 1,
             kh_str_hash_func, kh_str_hash_equal)

void *int_hash_get(ux_ihash_t *hash, khint32_t key) {
  khint_t iter = kh_get_int(hash, key);
  if (iter == kh_end(hash))
    return NULL;
  return kh_value(hash, iter);
}

void *str_hash_get(ux_shash_t *hash, const char *key) {
  khint_t iter = kh_get_str(hash, key);
  if (iter == kh_end(hash))
    return NULL;
  return kh_value(hash, iter);
}

void int_hash_set(ux_ihash_t *hash, khint32_t key, void *val) {
  int ret;
  khint_t iter = kh_put_int(hash, key, &ret);
  UX_ASSERT(ret > 0);
  kh_value(hash, iter) = val;
}

void str_hash_set(ux_shash_t *hash, const char *key, void *val) {
  int ret;
  khint_t iter = kh_put_str(hash, key, &ret);
  UX_ASSERT(ret > 0);
  kh_value(hash, iter) = val;
}

ux_list_t *int_hash_get_list(ux_list_ihash_t *hash, khint32_t key) {
  khint_t iter = kh_get_ilist(hash, key);
  if (iter == kh_end(hash)) {
    return NULL;
  }
  return (ux_list_t *)(&(kh_value(hash, iter)));
}

ux_list_t *str_hash_get_list(ux_list_shash_t *hash, const char *key) {
  khint_t iter = kh_get_slist(hash, key);
  if (iter == kh_end(hash)) {
    return NULL;
  }
  return (ux_list_t *)(&(kh_value(hash, iter)));
}

ux_list_t *int_hash_get_or_create_list(ux_list_ihash_t *hash,
                                          khint32_t key) {
  ux_list_t *list;
  khint_t iter;
  int ret;

  iter = kh_get_ilist(hash, key);
  if (iter == kh_end(hash)) {
    iter = kh_put_ilist(hash, key, &ret);
    UX_ASSERT(ret > 0);
    list = &(kh_value(hash, iter));
    QUEUE_INIT(&list->queue);
    return list;
  }
  list = &(kh_value(hash, iter));
  return list;
}

ux_list_t *str_hash_get_or_create_list(ux_list_shash_t *hash,
                                          const char *key) {
  ux_list_t *list;
  khint_t iter;
  int ret;

  iter = kh_get_slist(hash, key);
  if (iter == kh_end(hash)) {
    iter = kh_put_slist(hash, key, &ret);
    UX_ASSERT(ret > 0);
    list = &(kh_value(hash, iter));
    QUEUE_INIT(&list->queue);
    return list;
  }
  list = &(kh_value(hash, iter));
  return list;
}
