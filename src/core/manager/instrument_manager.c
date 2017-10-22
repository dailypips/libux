/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_manager.h"
#include "ux_internal.h"

// TODO: instrument manager init
/* instrument manager */
void ux_instrument_manager_init(ux_ctx_t *ctx) {
  kv_init(&ctx->instrument_by_id);
  ctx->instrument_by_symbol = khash_init(str);
  ctx->next_instrument_id = 0;
}

void ux_instrument_manager_destroy(ux_ctx_t *ctx) {
  // instrument_by_id nothing to do
  // instrument_by_symbol remove hashtable
  kh_destroy(str, ctx->instrument_by_symbol);
}

ux_instrument_t *ux_get_instrument_by_id(ux_ctx_t *ctx, int id) {
  return (ux_instrument_t *)kv_get(&ctx->instrument_by_id, id);
}

ux_instrument_t *ux_get_instrument_by_symbol(ux_ctx_t *ctx,
                                             const char *symbol) {
  khint_t iter = kh_get(str, ctx->instrument_by_symbol, symbol);
  if (iter == kh_end(ctx->instrument_by_symbol))
    return NULL;
  return kh_value(ctx->instrument_by_symbol, iter);
}

int ux_add_instrument(ux_ctx_t *ctx, ux_instrument_t *instrument) {
  int ret;
  ux_instrument_t *inst = ux_get_instrument_by_symbol(ctx, instrument->symbol);
  if (inst)
    return -1;
  instrument->id = ctx->next_instrument_id++;
  kv_set(&ctx->instrument_by_id, instrument->id, (uintptr_t)instrument);
  khint_t iter =
      kh_put_str(ctx->instrument_by_symbol, instrument->symbol, &ret);
  assert(ret == 0);
  kh_value(ctx->instrument_by_symbol, iter) = instrument;
  return 0;
}

void ux_order_manager_init(ux_ctx_t *ctx) {
  kv_init(&ctx->order_by_id);
  ctx->order_by_client_oid = khash_init(str);
  ctx->order_by_provider_oid = khash_init(str);
  ctx->order_list_by_oca = khash_init(list);
  ctx->next_order_id = 0;
}

void ux_order_manager_destroy(ux_ctx_t *ctx) {
  // order_by_id nothing to do
  // order_by_client_oid remove hashtable
  // order_by_provider_oid remove hashtable
  kh_destroy(str, ctx->order_by_client_oid);
  kh_destroy(str, ctx->order_by_provider_oid);
  // TODO:
  kh_destroy(list, ctx->order_list_by_oca);
}

ux_order_t *ux_get_order_by_id(ux_ctx_t *ctx, int id) {
  return (ux_order_t *)kv_get(&ctx->order_by_id, id);
}

void ux_add_order_by_id(ux_ctx_t *ctx, int id, ux_order_t *order) {
  kv_set(&ctx->order_by_id, id, order);
}

ux_order_t *ux_get_order_by_client_oid(ux_ctx_t *ctx, const char *oid) {
  return (ux_order_t *)hash_get(str, ctx->order_by_client_oid, oid);
  /*khint_t iter = kh_get(str, ctx->order_by_client_oid, oid);
  if (iter == kh_end(ctx->order_by_client_oid))
      return NULL;
  return kh_value(ctx->order_by_client_oid, iter);*/
}

void ux_add_order_by_client_oid(ux_ctx_t *ctx, const char *oid,
                                ux_order_t *order) {
  hash_set(str, ctx->order_by_client_oid, oid, order);
  /*int ret;
  khint_t iter = kh_put(str, ctx->order_by_client_oid, oid, &ret);
  UX_ASSERT(ret > 0);
  kh_value(ctx->order_by_client_oid, iter) = order;*/
}

ux_order_t *ux_get_order_by_provider_oid(ux_ctx_t *ctx, const char *oid) {
  return (ux_order_t *)(khint_t iter = kh_get(str, ctx->order_by_provider_oid, oid)) == kh_end(ctx->order_by_provider_oid)
                            ? 0
                            : kh_value(hash, iter);

  /*khint_t iter = kh_get(str, ctx->order_by_provider_oid, oid);
  if (iter == kh_end(ctx->order_by_provider_oid))
      return NULL;
  return kh_value(ctx->order_by_provider_oid, iter);*/
}

struct list_node_t {
  void *queue_node[2];
  void *order;
};

void ux_add_order_by_oca(ux_ctx_t *ctx, ux_order_t *order) {
  // ux_list_t* list = hash_get_or_create_list(slist, ctx->order_list_by_oca,
  // order->oca);
  ux_list_t *list = (ux_list_t *list = NULL, int ret = -2,
                     khint_t iter = kh_get(type, hash, key),
                     iter == kh_end(hash)
                         ? (iter = kh_put(type, hash, key, &ret),
                            assert(ret > 0), uxlist = &(kh_value(hash, iter)),
                            QUEUE_INIT(&uxlist->queue), uxlist)
                         : &(kh_value(hash, iter)));

  list_node_t *node = ux_zalloc(sizeof(*node));
  node->order = order;
  QUEUE_INSERT_TAIL(&list->queue, &node->queue_node);
}

ux_list_t *ux_get_order_list_by_oca(ux_ctx_t *ctx, const char *oca) {
  return shash_get_list(ctx->order_by_client_oid, oca);
}

int ux_add_order(ux_ctx_t *ctx, ux_order_t *order) {
  /*int ret;
  ux_order_t *inst = ux_get_order_by_symbol(ctx, order->symbol);
  if (inst)
      return -1;
  order->id = ctx->next_order_id++;
  kv_set(&ctx->order_by_id, order->id, (uintptr_t)order);
  khint_t iter = kh_put_str(ctx->order_by_symbol, order->symbol, &ret);
  assert(ret == 0);
  kh_value(ctx->order_by_symbol, iter) = order;*/
  return 0;
}

int ux_register_order(ux_ctx_t *ctx, ux_order_t *order) {
  if (order->id != -1)
    return UX_ORDER_HAS_REGISTERED;
  // TODO:lock
  // order->id = ctx->next_order_id++;
  order->id = ux_atomic_full_fetch_add(&ctx->next_order_id, 1);
  if (ctx->mode == UX_BUS_REALTIME && order->clOrderId == NULL) {
    char buf[256];
    ux_time_format(buf, 256, bus_get_time(ctx), 0);
    int len = strlen(buf);
    snprintf(buf + len, 256 - len - 1, " %d", order->id);
    order->clOrderId = ux_strdup(buf);
  }
}
void ux_reject_order(ux_ctx_t *ctx, ux_order_t *order) {
  order->orderStatus = UX_ORDER_STATUS_REJECTED;
}

int ux_send_order(ux_ctx_t *ctx, ux_order_t *order) {
  if (order->orderStatus != UX_ORDER_NOT_SENT)
    return UX_ORDER_SENT;
  uxe_order_sent_t *e = ux_event_malloc(UXE_ORDER_SENT);
  e->order = order;
  ux_dispatch_event(ctx, (ux_event_t *)e, UX_DISPATCH_IMMEDIATELY);
  if (order->id == -1)
    ux_register_order(ctx, order);

  if (order->oca != NULL) {
    ux_add_order_by_oca(ctx, order);
  }
  QUEUE_INSERT_TAIL(&ctx->orders, order);
  ux_add_order_by_id(ctx, order->id, order);
  if (ctx->mode == UX_BUS_REALTIME) {
    ux_add_order_by_client_oid(ctx, order->clOrderId, order);
  }
  order->time = bus_get_time(ctx);
  order->orderStatus = UX_ORDER_STATUS_PENDING_NEW;
  uxe_execution_command_t *command = ux_event_malloc(UXE_EXECUTION_COMMAND);
  command->ec_type = UX_EXECUTION_COMMAND_SEND;
  command->timestamp = order->time;
  command->id = order->id;
  command->order_id = order->id;
  command->client_order_id = ux_strdup(order->clOrderId);
  command->provider_order_id = ux_strdup(order->providerOrderId);
  command->provider_id = order->providerId;
  command->route_id = order->routeId;
  command->portfolio_id = order->portfolioId;
  command->transact_time = order->transactTime;
  command->instrument = order->instrument;
  command->instrument_id = order->instrumentId;
  command->execution_provider = order->execution_provider;
  command->portfolio = order->portfolio;
  command->order_side = order->orderSide;
  command->order_type = order->orderType;
  command->tif = order->tif;
  command->price = order->price;
  command->stop_price = order->stopPx;
  command->qty = order->qty;
  command->oca = ux_strdup(order->oca);
  command->text = ux_strdup(order->text);
  command->account = ux_strdup(order->account);
  command->clientID = ux_strdup(order->clientID);
  command->client_id = order->clientId;
  QUEUE_INSERT_TAIL(&ctx->execution_commands, command);
  ux_order_on_execution_command(order, command);
  ux_dispatch_event(ctx, command, UX_DISPATCH_IMMEDIATELY);
  uxe_order_pending_new_t *pending = ux_event_malloc(UXE_ORDER_PENDING_NEW);
  pending->order = order;
  ux_dispatch_event(ctx, pending, UX_DISPATCH_LATE);
  // TODO:save, risk, provider_send
}

int ux_cancel_order(ux_ctx_t *ctx, ux_order_t *order) {
  if (order->orderStatus != UX_ORDER_NOT_SENT)
    return UX_ORDER_SENT;

  uxe_execution_command_t *command = ux_event_malloc(UXE_EXECUTION_COMMAND);
  command->ec_type = UX_EXECUTION_COMMAND_CANCEL;
  command->timestamp = bus_get_time(ctx);
  command->order_id = order->id;
  command->client_order_id = ux_strdup(order->clOrderId);
  command->provider_order_id = ux_strdup(order->providerOrderId);
  command->provider_id = order->providerId;
  command->route_id = order->routeId;
  command->portfolio_id = order->portfolioId;
  command->transact_time = order->transactTime;
  command->instrument = order->instrument;
  command->instrument_id = order->instrumentId;
  command->execution_provider = order->execution_provider;
  command->portfolio = order->portfolio;
  command->order_side = order->orderSide;
  command->order_type = order->orderType;
  command->tif = order->tif;
  command->price = order->price;
  command->stop_price = order->stopPx;
  command->qty = order->qty;
  command->oca = ux_strdup(order->oca);
  command->text = ux_strdup(order->text);
  command->account = ux_strdup(order->account);
  command->clientID = ux_strdup(order->clientID);
  command->client_id = order->clientId;
  QUEUE_INSERT_TAIL(&ctx->execution_commands, command);
  ux_order_on_execution_command(order, command);
  ux_dispatch_event(ctx, command, UX_DISPATCH_IMMEDIATELY);
  // TODO: save, send
  // order->execution_provider.send(command);
}

int ux_replace_order(ux_ctx_t *ctx, ux_order_t *order, double price,
                     double stop_price, double qty) {
  if (order->orderStatus != UX_ORDER_NOT_SENT)
    return UX_ORDER_SENT;
  uxe_execution_command_t *command = ux_event_malloc(UXE_EXECUTION_COMMAND);
  command->ec_type = command->ec_type = UX_EXECUTION_COMMAND_REPLACE;
  command->order = order;
  command->timestamp = bus_get_time(ctx);
  command->price = price;
  command->stop_price = stop_price;
  command->qty = qty;
  QUEUE_INSERT_TAIL(&ctx->execution_commands, command);
  ux_order_on_execution_command(order, command);
  ux_dispatch_event(ctx, command, UX_DISPATCH_IMMEDIATELY);
  // TODO:save send
}
