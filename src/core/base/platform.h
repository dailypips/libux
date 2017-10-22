#ifndef __UX_PLATFORM_H__
#define __UX_PLATFORM_H__

/* Get windows.h included everywhere (we need it) */
#if defined(_WIN64) || defined(WIN64) || defined(_WIN32) || defined(WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define GRPC_WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED
#define WIN32_LEAN_AND_MEAN
#endif /* WIN32_LEAN_AND_MEAN */

#ifndef NOMINMAX
#define GRPC_NOMINMX_WAS_NOT_DEFINED
#define NOMINMAX
#endif /* NOMINMAX */

#ifndef _WIN32_WINNT
#error \
    "Please compile grpc with _WIN32_WINNT of at least 0x600 (aka Windows Vista)"
#else /* !defined(_WIN32_WINNT) */
#if (_WIN32_WINNT < 0x0600)
#error \
    "Please compile grpc with _WIN32_WINNT of at least 0x600 (aka Windows Vista)"
#endif /* _WIN32_WINNT < 0x0600 */
#endif /* defined(_WIN32_WINNT) */

#include <windows.h>

#ifdef GRPC_WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED
#undef GRPC_WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED
#undef WIN32_LEAN_AND_MEAN
#endif /* GRPC_WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED */

#ifdef GRPC_NOMINMAX_WAS_NOT_DEFINED
#undef GRPC_NOMINMAX_WAS_NOT_DEFINED
#undef NOMINMAX
#endif /* GRPC_WIN32_LEAN_AND_MEAN_WAS_NOT_DEFINED */
#endif /* defined(_WIN64) || defined(WIN64) || defined(_WIN32) || \
          defined(WIN32) */

/* Override this file with one for your platform if you need to redefine
   things.  */

#if !defined(UX_NO_AUTODETECT_PLATFORM)
#if defined(_WIN64) || defined(WIN64) || defined(_WIN32) || defined(WIN32)
#if defined(_WIN64) || defined(WIN64)
#define UX_ARCH_64 1
#else
#define UX_ARCH_32 1
#endif
#define UX_PLATFORM_STRING "windows"
#define UX_WINDOWS 1
#define UX_WINDOWS_SUBPROCESS 1
#define UX_WINDOWS_ENV
#ifdef __MSYS__
#define UX_GETPID_IN_UNISTD_H 1
#define UX_MSYS_TMPFILE
#define UX_POSIX_LOG
#define UX_POSIX_STRING
#define UX_POSIX_TIME
#else
#define UX_GETPID_IN_PROCESS_H 1
#define UX_WINDOWS_TMPFILE
#define UX_WINDOWS_LOG
#define UX_WINDOWS_CRASH_HANDLER 1
#define UX_WINDOWS_STRING
#define UX_WINDOWS_TIME
#endif
#ifdef __GNUC__
#define UX_GCC_ATOMIC 1
#define UX_GCC_TLS 1
#else
#define UX_WINDOWS_ATOMIC 1
#define UX_MSVC_TLS 1
#endif
#elif defined(UX_MANYLINUX1)
// TODO(atash): manylinux1 is just another __linux__ but with ancient
// libraries; it should be integrated with the `__linux__` definitions below.
#define UX_PLATFORM_STRING "manylinux"
#define UX_POSIX_CRASH_HANDLER 1
#define UX_CPU_POSIX 1
#define UX_GCC_ATOMIC 1
#define UX_GCC_TLS 1
#define UX_LINUX 1
#define UX_LINUX_LOG 1
#define UX_SUPPORT_CHANNELS_FROM_FD 1
#define UX_LINUX_ENV 1
#define UX_POSIX_TMPFILE 1
#define UX_POSIX_STRING 1
#define UX_POSIX_SUBPROCESS 1
#define UX_POSIX_SYNC 1
#define UX_POSIX_TIME 1
#define UX_GETPID_IN_UNISTD_H 1
#ifdef _LP64
#define UX_ARCH_64 1
#else /* _LP64 */
#define UX_ARCH_32 1
#endif /* _LP64 */
#elif defined(ANDROID) || defined(__ANDROID__)
#define UX_PLATFORM_STRING "android"
#define UX_ANDROID 1
#ifdef _LP64
#define UX_ARCH_64 1
#else /* _LP64 */
#define UX_ARCH_32 1
#endif /* _LP64 */
#define UX_CPU_POSIX 1
#define UX_GCC_SYNC 1
#define UX_GCC_TLS 1
#define UX_POSIX_ENV 1
#define UX_POSIX_TMPFILE 1
#define UX_ANDROID_LOG 1
#define UX_POSIX_STRING 1
#define UX_POSIX_SUBPROCESS 1
#define UX_POSIX_SYNC 1
#define UX_POSIX_TIME 1
#define UX_GETPID_IN_UNISTD_H 1
#define UX_SUPPORT_CHANNELS_FROM_FD 1
#elif defined(__linux__)
#define UX_PLATFORM_STRING "linux"
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <features.h>
#define UX_CPU_LINUX 1
#define UX_GCC_ATOMIC 1
#define UX_GCC_TLS 1
#define UX_LINUX 1
#define UX_LINUX_LOG
#define UX_SUPPORT_CHANNELS_FROM_FD 1
#define UX_LINUX_ENV 1
#define UX_POSIX_TMPFILE 1
#define UX_POSIX_STRING 1
#define UX_POSIX_SUBPROCESS 1
#define UX_POSIX_SYNC 1
#define UX_POSIX_TIME 1
#define UX_GETPID_IN_UNISTD_H 1
#ifdef _LP64
#define UX_ARCH_64 1
#else /* _LP64 */
#define UX_ARCH_32 1
#endif /* _LP64 */
#ifdef __GLIBC__
#define UX_POSIX_CRASH_HANDLER 1
#else /* musl libc */
#define UX_MUSL_LIBC_COMPAT 1
#endif
#elif defined(__APPLE__)
#include <Availability.h>
#include <TargetConditionals.h>
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#if TARGET_OS_IPHONE
#define UX_FORBID_UNREACHABLE_CODE 1
#define UX_PLATFORM_STRING "ios"
#define UX_CPU_IPHONE 1
#define UX_PTHREAD_TLS 1
#else /* TARGET_OS_IPHONE */
#define UX_PLATFORM_STRING "osx"
#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
#if __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_7
#define UX_CPU_IPHONE 1
#define UX_PTHREAD_TLS 1
#else /* __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_7 */
#define UX_CPU_POSIX 1
#define UX_GCC_TLS 1
#endif
#else /* __MAC_OS_X_VERSION_MIN_REQUIRED */
#define UX_CPU_POSIX 1
#define UX_GCC_TLS 1
#endif
#define UX_POSIX_CRASH_HANDLER 1
#endif
#define UX_APPLE 1
#define UX_GCC_ATOMIC 1
#define UX_POSIX_LOG 1
#define UX_POSIX_ENV 1
#define UX_POSIX_TMPFILE 1
#define UX_POSIX_STRING 1
#define UX_POSIX_SUBPROCESS 1
#define UX_POSIX_SYNC 1
#define UX_POSIX_TIME 1
#define UX_GETPID_IN_UNISTD_H 1
#define UX_SUPPORT_CHANNELS_FROM_FD 1
#ifdef _LP64
#define UX_ARCH_64 1
#else /* _LP64 */
#define UX_ARCH_32 1
#endif /* _LP64 */
#elif defined(__FreeBSD__)
#define UX_PLATFORM_STRING "freebsd"
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#define UX_FREEBSD 1
#define UX_CPU_POSIX 1
#define UX_GCC_ATOMIC 1
#define UX_GCC_TLS 1
#define UX_POSIX_LOG 1
#define UX_POSIX_ENV 1
#define UX_POSIX_TMPFILE 1
#define UX_POSIX_STRING 1
#define UX_POSIX_SUBPROCESS 1
#define UX_POSIX_SYNC 1
#define UX_POSIX_TIME 1
#define UX_GETPID_IN_UNISTD_H 1
#define UX_SUPPORT_CHANNELS_FROM_FD 1
#ifdef _LP64
#define UX_ARCH_64 1
#else /* _LP64 */
#define UX_ARCH_32 1
#endif /* _LP64 */
#elif defined(__native_client__)
#define UX_PLATFORM_STRING "nacl"
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define UX_NACL 1
#define UX_CPU_POSIX 1
#define UX_GCC_ATOMIC 1
#define UX_GCC_TLS 1
#define UX_POSIX_LOG 1
#define UX_POSIX_ENV 1
#define UX_POSIX_TMPFILE 1
#define UX_POSIX_STRING 1
#define UX_POSIX_SUBPROCESS 1
#define UX_POSIX_SYNC 1
#define UX_POSIX_TIME 1
#define UX_GETPID_IN_UNISTD_H 1
#ifdef _LP64
#define UX_ARCH_64 1
#else /* _LP64 */
#define UX_ARCH_32 1
#endif /* _LP64 */
#else
#error "Could not auto-detect platform"
#endif
#endif /* UX_NO_AUTODETECT_PLATFORM */

#if defined(__has_include)
#if __has_include(<atomic>)
#define GRPC_HAS_CXX11_ATOMIC
#endif /* __has_include(<atomic>) */
#endif /* defined(__has_include) */

#ifndef UX_PLATFORM_STRING
#warning "UX_PLATFORM_STRING not auto-detected"
#define UX_PLATFORM_STRING "unknown"
#endif

#ifdef UX_GCOV
#undef UX_FORBID_UNREACHABLE_CODE
#define UX_FORBID_UNREACHABLE_CODE 1
#endif

#ifdef _MSC_VER
#ifdef _PYTHON_MSVC
// The Python 3.5 Windows runtime is missing InetNtop
#define UX_WIN_INET_NTOP
#endif  // _PYTHON_MSVC
#if _MSC_VER < 1700
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif /* _MSC_VER < 1700 */
#else
#include <stdint.h>
#endif /* _MSC_VER */

/* Cache line alignment */
#ifndef UX_CACHELINE_SIZE_LOG
#if defined(__i386__) || defined(__x86_64__)
#define UX_CACHELINE_SIZE_LOG 6
#endif
#ifndef UX_CACHELINE_SIZE_LOG
/* A reasonable default guess. Note that overestimates tend to waste more
   space, while underestimates tend to waste more time. */
#define UX_CACHELINE_SIZE_LOG 6
#endif /* UX_CACHELINE_SIZE_LOG */
#endif /* UX_CACHELINE_SIZE_LOG */

#define UX_CACHELINE_SIZE (1 << UX_CACHELINE_SIZE_LOG)

/* scrub GCC_ATOMIC if it's not available on this compiler */
#if defined(UX_GCC_ATOMIC) && !defined(__ATOMIC_RELAXED)
#undef UX_GCC_ATOMIC
#define UX_GCC_SYNC 1
#endif

/* Validate platform combinations */
#if defined(UX_GCC_ATOMIC) + defined(UX_GCC_SYNC) + \
        defined(UX_WINDOWS_ATOMIC) !=                \
    1
#error Must define exactly one of UX_GCC_ATOMIC, UX_GCC_SYNC, UX_WINDOWS_ATOMIC
#endif

#if defined(UX_ARCH_32) + defined(UX_ARCH_64) != 1
#error Must define exactly one of UX_ARCH_32, UX_ARCH_64
#endif

#if defined(UX_CPU_LINUX) + defined(UX_CPU_POSIX) + defined(UX_WINDOWS) + \
        defined(UX_CPU_IPHONE) + defined(UX_CPU_CUSTOM) !=                 \
    1
#error Must define exactly one of UX_CPU_LINUX, UX_CPU_POSIX, UX_WINDOWS, UX_CPU_IPHONE, UX_CPU_CUSTOM
#endif

#if defined(UX_MSVC_TLS) + defined(UX_GCC_TLS) + defined(UX_PTHREAD_TLS) + \
        defined(UX_CUSTOM_TLS) !=                                            \
    1
#error Must define exactly one of UX_MSVC_TLS, UX_GCC_TLS, UX_PTHREAD_TLS, UX_CUSTOM_TLS
#endif

/* maximum alignment needed for any type on this platform, rounded up to a
   power of two */
#define UX_MAX_ALIGNMENT 16

#ifndef GRPC_ARES
#ifdef UX_WINDOWS
#define GRPC_ARES 0
#else
#define GRPC_ARES 1
#endif
#endif

#ifndef GRPC_MUST_USE_RESULT
#if defined(__GNUC__) && !defined(__MINGW32__)
#define GRPC_MUST_USE_RESULT __attribute__((warn_unused_result))
#define UX_ALIGN_STRUCT(n) __attribute__((aligned(n)))
#else
#define GRPC_MUST_USE_RESULT
#define UX_ALIGN_STRUCT(n)
#endif
#endif

#ifndef UX_PRINT_FORMAT_CHECK
#ifdef __GNUC__
#define UX_PRINT_FORMAT_CHECK(FORMAT_STR, ARGS) \
  __attribute__((format(printf, FORMAT_STR, ARGS)))
#else
#define UX_PRINT_FORMAT_CHECK(FORMAT_STR, ARGS)
#endif
#endif /* UX_PRINT_FORMAT_CHECK */

#if UX_FORBID_UNREACHABLE_CODE
#define UX_UNREACHABLE_CODE(STATEMENT)
#else
#define UX_UNREACHABLE_CODE(STATEMENT)             \
  do {                                              \
    ux_log(UX_ERROR, "Should never reach here."); \
    abort();                                        \
    STATEMENT;                                      \
  } while (0)
#endif /* UX_FORBID_UNREACHABLE_CODE */

#ifndef UXAPI
#define UXAPI
#endif

#ifndef GRPCAPI
#define GRPCAPI UXAPI
#endif

#ifndef CENSUSAPI
#define CENSUSAPI GRPCAPI
#endif

#ifndef UX_ATTRIBUTE_NO_TSAN /* (1) */
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define UX_ATTRIBUTE_NO_TSAN __attribute__((no_sanitize("thread")))
#endif                        /* __has_feature(thread_sanitizer) */
#endif                        /* defined(__has_feature) */
#ifndef UX_ATTRIBUTE_NO_TSAN /* (2) */
#define UX_ATTRIBUTE_NO_TSAN
#endif /* UX_ATTRIBUTE_NO_TSAN (2) */
#endif /* UX_ATTRIBUTE_NO_TSAN (1) */

#endif /* __UX_PLATFORM_H__ */
