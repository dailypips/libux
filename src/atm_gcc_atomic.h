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

#ifndef __UX_ATM_GCC_ATOMIC_H__
#define __UX_ATM_GCC_ATOMIC_H__

/* atm_platform.h for gcc and gcc-like compilers with the
   __atomic_* interface.  */
#include "ux_platform.h"

typedef intptr_t ux_atomic_t;
#define UX_ATM_MAX INTPTR_MAX

#ifdef UX_LOW_LEVEL_COUNTERS
extern ux_atomic_t ux_counter_atm_cas;
extern ux_atomic_t ux_counter_atm_add;
#define UX_ATM_INC_COUNTER(counter) \
  __atomic_fetch_add(&counter, 1, __ATOMIC_RELAXED)
#define UX_ATM_INC_CAS_THEN(blah) \
  (UX_ATM_INC_COUNTER(ux_counter_atm_cas), blah)
#define UX_ATM_INC_ADD_THEN(blah) \
  (UX_ATM_INC_COUNTER(ux_counter_atm_add), blah)
#else
#define UX_ATM_INC_CAS_THEN(blah) blah
#define UX_ATM_INC_ADD_THEN(blah) blah
#endif

#define ux_atomic_full_barrier() (__atomic_thread_fence(__ATOMIC_SEQ_CST))

#define ux_atomic_acq_load(p) (__atomic_load_n((p), __ATOMIC_ACQUIRE))
#define ux_atomic_no_barrier_load(p) (__atomic_load_n((p), __ATOMIC_RELAXED))
#define ux_atomic_rel_store(p, value) \
  (__atomic_store_n((p), (intptr_t)(value), __ATOMIC_RELEASE))
#define ux_atomic_no_barrier_store(p, value) \
  (__atomic_store_n((p), (intptr_t)(value), __ATOMIC_RELAXED))

#define ux_atomic_no_barrier_fetch_add(p, delta) \
  UX_ATM_INC_ADD_THEN(                        \
      __atomic_fetch_add((p), (intptr_t)(delta), __ATOMIC_RELAXED))
#define ux_atomic_full_fetch_add(p, delta) \
  UX_ATM_INC_ADD_THEN(                  \
      __atomic_fetch_add((p), (intptr_t)(delta), __ATOMIC_ACQ_REL))

static __inline int ux_atomic_no_barrier_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
  return UX_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
}

static __inline int ux_atomic_acq_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
  return UX_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED));
}

static __inline int ux_atomic_rel_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
  return UX_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_RELEASE, __ATOMIC_RELAXED));
}

static __inline int ux_atomic_full_cas(ux_atomic_t *p, ux_atomic_t o, ux_atomic_t n) {
  return UX_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED));
}

#define ux_atomic_full_xchg(p, n) \
  UX_ATM_INC_CAS_THEN(__atomic_exchange_n((p), (n), __ATOMIC_ACQ_REL))

#endif /* __UX_ATM_GCC_ATOMIC_H__ */
