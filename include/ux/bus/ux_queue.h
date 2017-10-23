/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_QUEUE_H__
#define __UX_QUEUE_H__

#include <ux/base/ux_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* queue module */
typedef enum {
    UX_CATEGORY_MARKET = 0,
    UX_CATEGORY_EXECUTION,
    UX_CATEGORY_SERVICE,
    UX_CATEGORY_COMMAND,
    UX_CATEGORY_LAST
}ux_queue_category;

typedef struct ux_queue_s ux_queue_t;

UX_EXTERN ux_queue_t* ux_queue_new(size_t size, ux_queue_category category);
UX_EXTERN int ux_queue_push(ux_queue_t *q, void *e);


#ifdef __cplusplus
}
#endif

#endif /*__UX_QUEUE_H__*/