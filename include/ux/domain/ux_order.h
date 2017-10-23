/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#ifndef __UX_ORDER_H__
#define __UX_ORDER_H__

#include <ux/base/ux_type.h>
#include <ux/domain/ux_provider.h>

#ifdef __cplusplus
extern "C" {
#endif

/* order module */

typedef enum {
    UX_EXEC_NEW,
    UX_EXEC_STOPPED,
    UX_EXEC_REJECTED,
    UX_EXEC_EXPIRED,
    UX_EXEC_TRADE,
    UX_EXEC_PENDING_CANCEL,
    UX_EXEC_CANCELLED,
    UX_EXEC_CANCEL_REJECT,
    UX_EXEC_PENDING_REPLACE,
    UX_EXEC_REPLACE,
    UX_EXEC_REPLACE_REJECT,
    UX_EXEC_TRADE_CORRECT,
    UX_EXEC_TRADE_CANCEL,
    UX_EXEC_ORDER_STATUS,
    UX_EXEC_PENDING_NEW,
    UX_EXEC_CLEARING_HOLD
} ux_exec_type;

typedef enum {
    UX_ORDER_TYPE_MARKET,
    UX_ORDER_TYPE_STOP,
    UX_ORDER_TYPE_LIMIT,
    UX_ORDER_TYPE_STOP_LIMIT,
    UX_ORDER_TYPE_MARKET_CLOSE,
    UX_ORDER_TYPE_PEGGED,
    UX_ORDER_TYPE_TRAILING_STOP,
    UX_ORDER_TYPE_TRAILING_STOP_LIMIT
} ux_order_type;

typedef enum {
    UX_ORDER_STATUS_NOT_SENT,
    UX_ORDER_STATUS_PENDING_NEW,
    UX_ORDER_STATUS_NEW,
    UX_ORDER_STATUS_REJECTED,
    UX_ORDER_STATUS_PARTIALLY_FILLED,
    UX_ORDER_STATUS_FILLED,
    UX_ORDER_STATUS_PENDING_CANCEL,
    UX_ORDER_STATUS_CANCELLED,
    UX_ORDER_STATUS_EXPIRED,
    UX_ORDER_STATUS_PENDING_REPLACE,
    UX_ORDER_STATUS_REPLACED
} ux_order_status;

typedef enum {
    UX_TIF_ATC,
    UX_TIF_DAY,
    UX_TIF_GTC,
    UX_TIF_IOC,
    UX_TIF_OPG,
    UX_TIF_OC,
    UX_TIF_FOK,
    UX_TIF_GTX,
    UX_TIF_GTD,
    UX_TIF_GFS,
    UX_TIF_AUC
} ux_tif;

typedef enum {
    UX_ORDER_SIDE_BUY,
    UX_ORDER_SIDE_SELL
} ux_order_side;


struct ux_order_s {
    ux_atomic_t refcount;
    ux_time_t time;
    ux_oid_t id;
    int clId;
    ux_id_t algoId;
    int clientId;
    char* providerOrderId;
    char* clOrderId;
    ux_pid_t providerId;
    ux_pid_t routeId;
    ux_execution_provider_t *execution_provider;
    ux_instrument_t *instrument;
    ux_iid_t instrumentId;
    ux_portfolio_t *portfolio;
    ux_id_t portfolioId;
    ux_id_t strategyId;
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
};

UX_EXTERN void ux_order_init(ux_order_t *order);
UX_EXTERN void ux_order_destroy(ux_order_t *order);
UX_EXTERN void ux_order_ref(ux_order_t *order);
UX_EXTERN void ux_order_unref(ux_order_t *order);

ux_order_t* ux_get_order_by_id(ux_ctx_t *ctx, int id);
ux_order_t* ux_get_order_by_client_order_id(ux_ctx_t *ctx, const char *id); /* client order id */
ux_order_t* ux_get_order_by_provider_order_id(ux_ctx_t *ctx, const char *id); // provider order id

int ux_register_order(ux_ctx_t *ctx, ux_order_t *order);

int ux_send_order(ux_ctx_t *ctx, ux_order_t *order);
void ux_reject_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_cancel_order(ux_ctx_t *ctx, ux_order_t *order);
int ux_replace_order(ux_ctx_t *ctx, ux_order_t *order, double price, double stop_price, double qty);

#ifdef __cplusplus
}
#endif

#endif // __UX_ORDER_H__
