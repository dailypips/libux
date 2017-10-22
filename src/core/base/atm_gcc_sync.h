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

#ifndef __UX_ATM_GCC_SYNC_H__
#define __UX_ATM_GCC_SYNC_H__

/* variant of atm_platform.h for gcc and gcc-like compiers with __sync_*
   interface */
#include "platform.h"

typedef intptr_t ux_atomic_t;
#define UX_ATM_MAX INTPTR_MAX

#define UX_ATM_COMPILE_BARRIER_() __asm__ __volatile__("" : : : "memory")

#if defined(__i386) || defined(__x86_64__)
/* All loads are acquire loads and all stores are release stores.  */
#define UX_ATM_LS_BARRIER_() UX_ATM_COMPILE_BARRIER_()
#else
#define UX_ATM_LS_BARRIER_() ux_atomic_full_barrier()
#endif

#define ux_atomic_full_barrier() (__sync_synchronize())

static __inline ux_atomic_t ux_atomic_acq_load(const ux_atomic_t *p) {
  ux_atomic_t value = *p;
  UX_ATM_LS_BARRIER_();
  return value;
}

static __inline ux_atomic_t ux_atomic_no_barrier_load(const ux_atomic_t *p) {
  ux_atomic_t value = *p;
  UX_ATM_COMPILE_BARRIER_();
  return value;
}

static __inline void ux_atomic_rel_store(ux_atomic_t *p, ux_atomic_t value) {
  UX_ATM_LS_BARRIER_();
  *p = value;
}

static __inline void ux_atomic_no_barrier_store(ux_atomic_t *p, ux_atomic_t value) {
  UX_ATM_COMPILE_BARRIER_();
  *p = value;
}

#undef UX_ATM_LS_BARRIER_
#undef UX_ATM_COMPILE_BARRIER_

#define ux_atomic_no_barrier_fetch_add(p, delta) \
  ux_atomic_full_fetch_add((p), (delta))
#define ux_atomic_full_fetch_add(p, delta) (__sync_fetch_and_add((p), (delta)))

#define ux_atomic_no_barrier_cas(p, o, n) ux_atomic_acq_cas((p), (o), (n))
#define ux_atomic_acq_cas(p, o, n) (__sync_bool_compare_and_swap((p), (o), (n)))
#define ux_atomic_rel_cas(p, o, n) ux_atomic_acq_cas((p), (o), (n))
#define ux_atomic_full_cas(p, o, n) ux_atomic_acq_cas((p), (o), (n))

static __inline ux_atomic_t ux_atomic_full_xchg(ux_atomic_t *p, ux_atomic_t n) {
  ux_atomic_t cur;
  do {
    cur = ux_atomic_acq_load(p);
  } while (!ux_atomic_rel_cas(p, cur, n));
  return cur;
}

#endif /* __UX_ATM_GCC_SYNC_H__ */
