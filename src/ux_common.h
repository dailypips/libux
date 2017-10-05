#ifndef __UX_COMMON_H__
#define __UX_COMMON_H__

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define UX_ASSERT(x) assert((x))
#define UX_PRINT_FOMAT_CHECK(x,y)

#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))


#if defined(_WIN64) || defined(WIN64) || defined(_WIN32) || defined(WIN32)
#  define CORE_EXPORT __declspec(dllexport)
#  define CORE_IMPORT extern __declspec(dllimport)
#  define CORE_HIDDEN
#elif defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#  define CORE_EXPORT
#  define CORE_IMPORT extern
#  define CORE_HIDDEN __attribute__ ((visibility ("hidden")))
#else
#  define CORE_EXPORT
#  define CORE_IMPORT extern
#  define CORE_HIDDEN
#endif

#if defined(UX_BUILD_SHARED)
#if defined(UX_BUILD_LIBRARY)
#define UX_EXPORT		CORE_EXPORT
#else
#define UX_EXPORT		CORE_IMPORT
#endif
#else
#define UX_EXPORT		extern
#endif

#define UX_HIDDEN CORE_HIDDEN

#if defined(_MSC_VER)
/* MSVC is stuck in the last century and doesn't have C99's stdint.h. */
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#ifdef _WIN64
typedef __int64 intptr_t;
typedef unsigned __int64 uintptr_t;
#else
typedef __int32 intptr_t;
typedef unsigned __int32 uintptr_t;
#endif
#else
#include <stdint.h>
#endif

/* Needed everywhere. */
#include <string.h>
#include <stdlib.h>

/* Various macros. */
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

/* Optional defines. */
#ifndef UX_NORET
#define UX_NORET
#endif
#ifndef UX_NOAPI
#define UX_NOAPI	extern
#endif
#ifndef UX_LIKELY
#define UX_LIKELY(x)	(x)
#define UX_UNLIKELY(x)	(x)
#endif

/* Attributes for internal functions. */
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


typedef uint64_t datetime_t;

typedef enum {
    UX_CATEGORY_MARKET = 0,
    UX_CATEGORY_EXECUTION,
    UX_CATEGORY_SERVICE,
    UX_CATEGORY_COMMAND,
    UX_CATEGORY_LAST
}ux_queue_category;

typedef enum {
    UX_CLOCK_LOCAL  = 0,
    UX_CLOCK_EXCHANGE,
    UX_CLOCK_LAST
}ux_clock_type;

#endif // __UX_COMMON_H__
