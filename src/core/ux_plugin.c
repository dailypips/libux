/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include <cp.h>
#include "ux_internal.h"

/* define ext point for uxcore */

#define UX_BAR_FACTORY_ITEM_EXT_POINT         "org.dailypips.uxcore.barfactory.item"
#define UX_DATA_PROVIDER_EXT_POINT            "org.dailypips.uxcore.provider.data"
#define UX_INSTRUMENT_PROVIDER_EXT_POINT      "org.dailypips.uxcore.provider.instrument"
#define UX_FUNDAMENTAL_PROVIDER_EXT_POINT     "org.dailypips.uxcore.provider.fundamental"
#define UX_NEWS_PROVIDER_EXT_POINT            "org.dailypips.uxcore.provider.news"

#define UX_EXECUTION_PROVIDER_EXT_POINT       "org.dailypips.uxcore.provider.execution"

#define UX_HISTORICAL_PROVIDER_EXT_POINT      "org.dailypips.uxcore.provider.historical"


int subscribe(cp_context_t *ctx, const char *extpt_id, cp_extension_t ***info, int *num)
{
    cp_status_t status;
    *info = cp_get_extensions_info(ctx, extpt_id, &status, num);
    return status == CP_OK;
}
