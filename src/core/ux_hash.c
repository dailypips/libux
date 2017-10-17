#include "ux_hash.h"

__KHASH_IMPL(int, kh_inline klib_unused, khint32_t, void*, 1, kh_int_hash_func, kh_int_hash_equal)
__KHASH_IMPL(str, kh_inline klib_unused, kh_cstr_t, void*, 1, kh_str_hash_func, kh_str_hash_equal)
__KHASH_IMPL(list, kh_inline klib_unused, khint32_t, _list_t, 1, kh_int_hash_func, kh_int_hash_equal)
