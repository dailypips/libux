/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_COMMON_H__
#define __UX_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  /* Windows - set up dll import/export decorators. */
# if defined(BUILDING_UX_SHARED)
    /* Building shared library. */
#   define UX_EXTERN __declspec(dllexport)
# elif defined(USING_UX_SHARED)
    /* Using shared library. */
#   define UX_EXTERN __declspec(dllimport)
# else
    /* Building static library. */
#   define UX_EXTERN /* nothing */
# define UX_HIDDEN
# endif
#elif __GNUC__ >= 4
# define UX_EXTERN __attribute__((visibility("default")))
# define UX_HIDDEN __attribute__ ((visibility ("hidden")))
#else
# define UX_EXTERN /* nothing */
#endif


#include <stddef.h>

#if defined(_MSC_VER) && _MSC_VER < 1600
# include "stdint-msvc2008.h"
#else
# include <stdint.h>
#endif

#include <assert.h>
#define UX_ASSERT(x) assert((x))
#define UX_PRINT_FOMAT_CHECK(x,y)

#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))


#ifndef UX_UNUSED
#define UX_UNUSED(x)	((void)(x))	/* to avoid warnings */
#endif


#if defined(__GNUC__)

#define UX_NORET	__attribute__((noreturn))
#define UX_ALIGN(n)	__attribute__((aligned(n)))
#define UX_INLINE	inline
#define UX_AINLINE	inline __attribute__((always_inline))
#define UX_NOINLINE	__attribute__((noinline))

#define UX_LIKELY(x)	__builtin_expect(!!(x), 1)
#define UX_UNLIKELY(x)	__builtin_expect(!!(x), 0)

#elif defined(_MSC_VER)

#define UX_NORET	__declspec(noreturn)
#define UX_ALIGN(n)	__declspec(align(n))
#define UX_INLINE	__inline
#define UX_AINLINE	__forceinline
#define UX_NOINLINE	__declspec(noinline)

#else
#error "missing defines for your compiler"
#endif

#ifndef UX_LIKELY
#define UX_LIKELY(x)	(x)
#define UX_UNLIKELY(x)	(x)
#endif

/* Optional defines. */
#ifndef UX_NORET
#define UX_NORET
#endif
#ifndef UX_NOAPI
#define UX_NOAPI	extern
#endif
#if defined(UX_AMALG_C)
#define UX_FUNC		static
#else
#define UX_FUNC		UX_NOAPI
#endif

/* Static assertions. */
#define UX_ASSERT_NAME2(name, line)	name ## line
#define UX_ASSERT_NAME(line)		UX_ASSERT_NAME2(lj_assert_, line)
#ifdef __COUNTER__
#define UX_STATIC_ASSERT(cond) \
  extern void UX_ASSERT_NAME(__COUNTER__)(int STATIC_ASSERTION_FAILED[(cond)?1:-1])
#else
#define UX_STATIC_ASSERT(cond) \
  extern void UX_ASSERT_NAME(__LINE__)(int STATIC_ASSERTION_FAILED[(cond)?1:-1])
#endif


#ifdef __cplusplus
}
#endif

#endif // __UX_COMMON_H__
