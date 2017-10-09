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

/* atomic module */
typedef intptr_t ux_atomic_t;

#ifdef __cplusplus
}
#endif

#endif // __UX_COMMON_H__
