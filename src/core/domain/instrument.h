/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_instrument.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *ux_instrument_get_symbol(ux_instrument_t *instrument,
                                     ux_pid_t provider_id);
const char *ux_instrument_get_exchange(ux_instrument_t *instrument,
                                       ux_pid_t provider_id);
ux_currency_t ux_instrument_get_currency(ux_instrument_t *instrument,
                                         ux_pid_t provider_id);
ux_instrument_leg_t *ux_instrument_add_leg(ux_instrument_t *instrument,
                                           ux_instrument_t *leg, double weight);
void ux_instrument_remove_leg(ux_instrument_t *instrument,
                              ux_instrument_leg_t *leg);

void ux_instrument_add_alt(ux_instrument_t *instrument, ux_pid_t provider_id,
                           const char *symbol, const char *exchange,
                           ux_currency_t currency);
void ux_instrument_remove_alt(ux_instrument_t *instrument,
                              ux_pid_t provider_id);

#ifdef __cplusplus
}
#endif

#endif // __INSTRUMENT_H__
