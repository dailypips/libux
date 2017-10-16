/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef __UX_ORDER_H__
#define __UX_ORDER_H__

#include <ux/common.h>
#include <ux/datetime.h>
#include <ux/currency.h>

#ifdef __cplusplus
extern "C" {
#endif

/* order module */


typedef struct ux_order_s {
    ux_atomic_t refcount;
    int id;
    int clId;
    int algoId;
    int clientId;
    char* providerOrderId;
    char* clOrderId;
    uint8_t providerId;
    uint8_t routeId;
    int instrumentId;
    int portfolioId;
    int strategyId;
    ux_currency_t currencyId;
    ux_time_t transactTime;
    ux_order_status orderStatus;
    ux_order_side orderSide;
    ux_order_type orderType;
    ux_tif tif;
    ux_time_t expireTime;
    double price;
    double stopPx;
    double avgPx;
    double qty;
    double cumQty;
    double leavesQty;
    double minQty;
    double pegDifference;
    char* execInst;
    char* oca;
    char* text;
    char* account;
    char* clientID;
    double queueSize;
    int isOutPrice;
    int isQueueCalculated;
    void* commands[2]; // list
    void* reports[2]; // list
    //void* messages[2]; // list
    void* queue_node[2]; // for instrument manager
} ux_order_t;

UX_EXTERN void ux_order_init(ux_order_t *order);
UX_EXTERN void ux_order_destory(ux_order_t *order);
UX_EXTERN void ux_order_ref(ux_order_t *order);
UX_EXTERN void ux_order_unref(ux_order_t *order);

void get_order_by_id(int id);
void get_order_by_client_order_id(char *id); /* client order id */
void get_order_by_provider_order_id(char *id); // provider order id

void register_order(ux_order_t *order);

void send_order(ux_order_t *order);
void reject_order(ux_order_t *order);
void cancel_order(ux_order_t *order);
void replace_order(ux_order_t *order, double price, double stop_price, double qty);

void on_account_report();
void on_execution_report();

void delete_by_series_name(char *series_name);

void load(char *name /*null*/, int clientId /* -1 */);


#ifdef __cplusplus
}
#endif

#endif // __UX_ORDER_H__
