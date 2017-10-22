/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __UX_ATM_WINDOWS_H__
#define __UX_ATM_WINDOWS_H__

/** Win32 variant of atm_platform.h */
#include "platform.h"

typedef intptr_t ux_atomic_t;
#define UX_ATM_MAX INTPTR_MAX

#define ux_atomic_full_barrier MemoryBarrier

static __inline ux_atomic_t ux_atomic_acq_load(const ux_atomic_t *p) {
  ux_atomic_t result = *p;
  ux_atomic_full_barrier();
  return result;
}

static __inline ux_atomic_t ux_atomic_no_barrier_load(const ux_atomic_t *p) {
  /* TODO(dklempner): Can we implement something better here? */
  return ux_atomic_acq_load(p);
}

static __inline void ux_atomic_rel_store(ux_atomic_t *p, ux_atomic_t value) {
  ux_atomic_full_barrier();
  *p = value;
}

static __inline void ux_atomic_no_barrier_store(ux_atomic_t *p, ux_atomic_t value) {
  /* TODO(ctiller): Can we implement something better here? */
  ux_atomic_rel_store(p, value);
}

static __inline int ux_atomic_no_barrier_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
/** InterlockedCompareExchangePointerNoFence() not available on vista or
   windows7 */
#ifdef UX_ARCH_64
  return o == (ux_atomic_t)InterlockedCompareExchangeAcquire64(
                  (volatile LONGLONG *)p, (LONGLONG)n, (LONGLONG)o);
#else
  return o == (ux_atomic_t)InterlockedCompareExchangeAcquire((volatile LONG *)p,
                                                         (LONG)n, (LONG)o);
#endif
}

static __inline int ux_atomic_acq_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
#ifdef UX_ARCH_64
  return o == (ux_atomic_t)InterlockedCompareExchangeAcquire64(
                  (volatile LONGLONG *)p, (LONGLONG)n, (LONGLONG)o);
#else
  return o == (ux_atomic_t)InterlockedCompareExchangeAcquire((volatile LONG *)p,
                                                         (LONG)n, (LONG)o);
#endif
}

static __inline int ux_atomic_rel_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
#ifdef UX_ARCH_64
  return o == (ux_atomic_t)InterlockedCompareExchangeRelease64(
                  (volatile LONGLONG *)p, (LONGLONG)n, (LONGLONG)o);
#else
  return o == (ux_atomic_t)InterlockedCompareExchangeRelease((volatile LONG *)p,
                                                         (LONG)n, (LONG)o);
#endif
}

static __inline int ux_atomic_full_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
#ifdef UX_ARCH_64
  return o == (ux_atomic_t)InterlockedCompareExchange64((volatile LONGLONG *)p,
                                                    (LONGLONG)n, (LONGLONG)o);
#else
  return o == (ux_atomic_t)InterlockedCompareExchange((volatile LONG *)p, (LONG)n,
                                                  (LONG)o);
#endif
}

static __inline ux_atomic_t ux_atomic_no_barrier_fetch_add(ux_atomic_t *p,
                                                     ux_atomic_t delta) {
  /** Use the CAS operation to get pointer-sized fetch and add */
  ux_atomic_t old;
  do {
    old = *p;
  } while (!ux_atomic_no_barrier_cas(p, old, old + delta));
  return old;
}

static __inline ux_atomic_t ux_atomic_full_fetch_add(ux_atomic_t *p, ux_atomic_t delta) {
  /** Use a CAS operation to get pointer-sized fetch and add */
  ux_atomic_t old;
#ifdef UX_ARCH_64
  do {
    old = *p;
  } while (old != (ux_atomic_t)InterlockedCompareExchange64((volatile LONGLONG *)p,
                                                        (LONGLONG)old + delta,
                                                        (LONGLONG)old));
#else
  do {
    old = *p;
  } while (old != (ux_atomic_t)InterlockedCompareExchange(
                      (volatile LONG *)p, (LONG)old + delta, (LONG)old));
#endif
  return old;
}

static __inline ux_atomic_t ux_atomic_full_xchg(ux_atomic_t *p, ux_atomic_t n) {
  return (ux_atomic_t)InterlockedExchangePointer((PVOID *)p, (PVOID)n);
}

#endif /* __UX_ATM_WINDOWS_H__ */
