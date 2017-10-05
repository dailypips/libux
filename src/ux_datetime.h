/******************************************************************************
 * Quantitative Trading Library                                               *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 ******************************************************************************/

#ifndef __UX_DATETIME_H__
#define __UX_DATETIME_H__

#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include "ux_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* datetime module */

/* datetime type represents a date and time.
   that stores the date and time as the number of microsecond intervals since
   12:00 AM January 1, year 1 A.D (as Day 0). in the proleptic Gregorian Calendar.
*/

typedef uint64_t datetime_t;
typedef int64_t  timespan_t;

#define EPOCH_DATE_TIME UINT64_C(62135596800000000)  /* 1970-01-01T00:00:00:00 */
#define MIN_DATE_TIME UINT64_C(0)                    /* 0001-01-01T00:00:00:00 */
#define MAX_DATE_TIME UINT64_C(315537897599999999)   /* 9999-12-31T23:59:59.999999*/

UX_EXPORT datetime_t datetime_from_ymd(uint16_t year, uint16_t month, uint16_t day);
UX_EXPORT datetime_t datetime_from_hmsu(uint16_t hour, uint16_t minute, uint16_t second, uint32_t usec);
UX_EXPORT datetime_t datetime_from_timeval(struct timeval *t);
/* result = 0 means ok, other means error */
UX_EXPORT int datetime_from_iso8601(const char *str, size_t len, datetime_t *dt);
UX_EXPORT void datetime_decode(datetime_t dt, uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t *minute, uint16_t *second, uint32_t *usec);
UX_EXPORT int datetime_to_tm(const datetime_t dt, struct tm *tmp);
UX_EXPORT size_t datetime_format(char *dst, size_t len, datetime_t dt, long offset /* see timeout_offset */);
/* return timezone offset from utc, in minutes [-1439,1439] */
UX_EXPORT long get_timezone_offset(void);
UX_EXPORT void ux_timezone_update(void);
UX_EXPORT datetime_t datetime_now(void);

#ifdef __cplusplus
}
#endif

#endif // __UX_DATETIME_H__
