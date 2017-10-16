/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/

#include <ux/ux.h>


const char* currency_table[ISO4217_CURRENCY_LAST] = {
#define CLASSENUM(name, code)   ##name
    EVENTDEF(CLASSENUM)
#undef CLASSENUM
};

ux_currency_t ux_currency_from_name(const char *name);
const char* ux_currency_to_name(ux_currency_t currency);
