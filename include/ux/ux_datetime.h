/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

/* datetime module */

#ifndef __UX_DATE_TIME_H__
#define __UX_DATE_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ux/ux_common.h>
#include <sys/time.h> // timeval
#include <time.h>   // struct tm

/* datetime module */
/* datetime_t type represents a date and time.
   that stores the date and time as the number of microsecond intervals since
   12:00 AM January 1, year 1 A.D (as Day 0). in the proleptic Gregorian Calendar.
*/

typedef uint64_t ux_time_t;
typedef int64_t  ux_timespan_t;

#define EPOCH_DATE_TIME UINT64_C(62135596800000000)  /* 1970-01-01T00:00:00:00 */
#define MIN_DATE_TIME UINT64_C(0)                    /* 0001-01-01T00:00:00:00 */
#define MAX_DATE_TIME UINT64_C(315537897599999999)   /* 9999-12-31T23:59:59.999999*/

#define TICKS_PER_SECOND UINT64_C(1000000)
#define TICKS_PER_DAY UINT64_C(86400000000)

UX_EXTERN ux_time_t ux_time_from_ymd(uint16_t year, uint16_t month, uint16_t day);
UX_EXTERN ux_time_t ux_time_from_hmsu(uint16_t hour, uint16_t minute, uint16_t second, uint32_t usec);
UX_EXTERN ux_time_t ux_time_from_timeval(struct timeval *t);
/* result = 0 means ok, other means error */
UX_EXTERN int ux_time_from_iso8601(const char *str, size_t len, ux_time_t *dt);
UX_EXTERN void ux_time_decode(ux_time_t dt, uint16_t *year, uint16_t *month, uint16_t *day, uint16_t *hour, uint16_t *minute, uint16_t *second, uint32_t *usec);
UX_EXTERN int ux_time_to_tm(const ux_time_t dt, struct tm *tmp);
UX_EXTERN size_t ux_time_format(char *dst, size_t len, ux_time_t dt, long offset /* see timeout_offset */);
/* return timezone offset from utc, in minutes [-1439,1439] */
UX_EXTERN long ux_get_timezone_offset(void);
UX_EXTERN void ux_timezone_update(void);
UX_EXTERN ux_time_t ux_time_now(void);


#ifdef __cplusplus
}
#endif

#endif /*__UX_DATE_TIME_H__*/
