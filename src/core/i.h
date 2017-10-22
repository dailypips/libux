#ifndef __UX_INTERNAL_H__
#define __UX_INTERNAL_H__

#include "ux_atomic.h"
#include "ux_hash.h"
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <uv.h>
#include <ux/id_array.h>
#include <ux/ux.h>
#include <ux/ux_macro.h>

#ifdef __cplusplus
extern "C" {
#endif

/* event  module */

/* event class info */


/* queue module */
#include "ux_mpscq.h"
#include "ux_spscq.h"

struct ux_queue_s {
  void *data;
  /* private */
  ux_atomic_t refcount;
  spscq_t spsc;
  /* only access by consumer */
  void *ctx;
  ux_queue_category category;
  /* mpsc */
  mpscq_node mpsc_node;
  /* min heap node */
  void *heap_node[3];
  uint64_t start_id;
};

UX_FUNC ux_queue_t *ux_queue_init(ux_queue_t *q, unsigned int size,
                                  ux_queue_category category);
UX_FUNC void ux_queue_destroy(ux_queue_t *q);

UX_FUNC void ux_queue_ref(ux_queue_t *q);
UX_FUNC void ux_queue_unref(ux_queue_t *q);

UX_FUNC unsigned int ux_queue_size(ux_queue_t *q);
UX_FUNC unsigned int ux_queue_capacity(ux_queue_t *q);

UX_FUNC void *ux_queue_peek(const ux_queue_t *q);
UX_FUNC void *ux_queue_pop(ux_queue_t *q);

UX_FUNC int ux_queue_is_empty(ux_queue_t *q);
UX_FUNC int ux_queue_is_full(ux_queue_t *q);

/* bar factory */
// typedef struct ux_barfactory_s ux_ctx_t;
// typedef struct ux_barfactory_item_s ux_barfactory_item_t;

// typedef void (*ux_on_tick_cb)(ux_barfactory_item_t *item, ux_event_tick_t
// *tick);

UX_FUNC void ux_barfactory_item_init(ux_bar_generator_t *item);
UX_FUNC void ux_barfactory_item_destroy(ux_bar_generator_t *item);

#define BAR_GENERATOR_PUBLIC_FIELDS                                              \
  ux_list_hash_t *list_by_instrument_id;                                       \
  void *reminder_items;

struct ux_barfactory_s {
  BAR_GENERATOR_PUBLIC_FIELDS
};

UX_FUNC void ux_barfactory_init(ux_ctx_t *factory);
UX_FUNC void ux_barfactory_destroy(ux_ctx_t *factory);
UX_EXTERN void ux_barfactory_add_item(ux_ctx_t *ctx, ux_bar_generator_t *item);
UX_EXTERN void ux_barfactory_remove_item(ux_ctx_t *ctx,
                                         ux_bar_generator_t *item);
UX_EXTERN void ux_barfactory_add_reminder(ux_ctx_t *factory,
                                          uxe_reminder_t *reminder);
UX_EXTERN void bar_manager_process_tick(ux_ctx_t *ctx, ux_event_tick_t *e);
UX_EXTERN void ux_barfactory_emit_bar_open(ux_ctx_t *factory,
                                           ux_bar_generator_t *item);
UX_EXTERN void ux_barfactory_emit_bar(ux_ctx_t *factory,
                                      ux_bar_generator_t *item);

/* data manager module
#define DATA_MANAGER_FIELDS                                 \
    ux_ihash_t *dm_asks;                             \
    ux_ihash_t *dm_bids;                             \
    ux_ihash_t *dm_trades;
*/

/* ctx module */
typedef struct {
  void *min;
  unsigned int nelts;
} min_heap;

typedef struct risk_report_s {
  char *text;
} risk_report_t;

typedef struct risk_plugin_s {
  risk_report_t *(*on_execution_report)(ux_ctx_t *ctx,
                                        uxe_execution_report_t *report);
  // void (*on_property_changed)(ux_ctx_t *ctx, uxe_property_changed_t
  // *changed);
} risk_plugin_t;

#define LOOP_FIELDS                                                            \
  uv_mutex_t wait_mutex;                                                       \
  uv_cond_t wait_cond;                                                         \
  int stop_flag;                                                               \
  mpscq_t async_queue;

#define EVENT_BUS_FIELDS                                                       \
  ux_bus_mode mode;                                                            \
  mpscq_t pending_queue;                                                       \
  spscq_t buffed_event_queue;                                                  \
  min_heap queue_heap[UX_CATEGORY_LAST];                                       \
  min_heap timer_heap[UX_CLOCK_LAST];                                          \
  uint64_t counter;                                                            \
  ux_time_t time[UX_CLOCK_LAST];                                               \
  ux_event_t *saved_event;                                                     \
  ux_queue_t *attached[128];                                                   \
  int attached_count;                                                          \
  int is_simulator_stop;

#define INSTRUMENT_MANAGER_FIELDS                                              \
  ux_atomic_t next_instrument_id;                                              \
  ux_idarray_t instrument_by_id;                                               \
  ux_shash_t *instrument_by_symbol;                                            \
  void *instruments[2];

#define ORDER_MANAGER_FIELDS                                                   \
  ux_atomic_t next_order_id;                                                   \
  ux_idarray_t order_by_id;                                                    \
  ux_shash_t *order_by_client_oid;                                             \
  ux_shash_t *order_by_provider_oid;                                           \
  ux_list_shash_t *order_list_by_oca;                                          \
  void *orders[2];

struct ux_ctx_s {
  LOOP_FIELDS
  EVENT_BUS_FIELDS
  /* stat */
  uint64_t event_count;
  /* instrument manager*/
  INSTRUMENT_MANAGER_FIELDS
  ORDER_MANAGER_FIELDS
  /* bar factory */
  BAR_GENERATOR_PUBLIC_FIELDS
  // BAR_GENERATOR_PUBLIC_FIELDS
  DATA_MANAGER_FIELDS
  // STRATEGY_MANAGER_FIELDS
  /* simulator */

  ux_data_simulator_t *data_simulator;
  ux_execution_simulator_t *execution_simulator;
  /* logger */
  ux_event_logger_t *event_logger;
};


/* default event dispatch */
UX_FUNC void event_reminder_dispatch(ux_ctx_t *ctx, ux_event_t *e);
UX_FUNC void event_ask_dispatch(ux_ctx_t *ctx, ux_event_t *e);
UX_FUNC void event_bid_dispatch(ux_ctx_t *ctx, ux_event_t *e);
UX_FUNC void event_trade_dispatch(ux_ctx_t *ctx, ux_event_t *e);

/* series & indicator */
typedef struct ux_series_s ux_series_t;

struct ux_series_s {
  void *indicators[2];
  const char *(*get_name)(ux_series_t *s);
  const char *(*get_description)(ux_series_t *s);
  int (*get_size)(ux_series_t *s);
  double (*get_first)(ux_series_t *s);
  double (*get_last)(ux_series_t *s);
  ux_time_t (*get_first_time)(ux_series_t *s);
  ux_time_t (*get_last_time)(ux_series_t *s);
  double (*get)(ux_series_t, int index);
  double (*get_min)(ux_series_t *s, ux_time_t start, ux_time_t end);
  double (*get_max)(ux_series_t *s, ux_time_t start, ux_time_t end);
};

typedef struct {
  ux_time_t timestamp;
  double value;
} ux_time_series_data_t;

void sm_on_strategy_event(ux_ctx_t *ctx, void *data);
void sm_on_exception(ux_ctx_t *ctx, const char *source, ux_event_t *e,
                     uxe_exception_t *exception);
void sm_on_instrument_added(ux_ctx_t *ctx, ux_instrument_t *instrument);
void sm_on_instrument_removed(ux_ctx_t *ctx, ux_instrument_t *instrument);
void sm_on_reminder(ux_ctx_t *ctx, ux_time_t dateTime, void *data);
void sm_on_exchange_reminder(ux_ctx_t *ctx, ux_time_t dateTime, void *data);
void sm_on_provider_connected(ux_ctx_t *ctx, ux_provider_t *provider);
void sm_on_provider_disconnected(ux_ctx_t *ctx, ux_provider_t *provider);
void sm_on_provider_error(ux_ctx_t *ctx, uxe_provider_error_t *error);
void sm_on_bid(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_bid_t *bid);
void sm_on_ask(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_ask_t *ask);
void sm_on_trade(ux_ctx_t *ctx, ux_instrument_t *instrument,
                 uxe_trade_t *trade);
void sm_on_level2_snapshot(ux_ctx_t *ctx, ux_instrument_t *instrument,
                           uxe_level2_snapshot_t *snapshot);
void sm_on_Level2_update(ux_ctx_t *ctx, ux_instrument_t *instrument,
                         uxe_level2_update_t *update);
void sm_on_bar_open(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_bar_t *bar);
void sm_on_bar(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_bar_t *bar);
void sm_on_bar_slice(ux_ctx_t *ctx, uxe_bar_slice_t *slice);
void sm_on_news(ux_ctx_t *ctx, ux_instrument_t *instrument, uxe_news_t *news);
void sm_on_fundamental(ux_ctx_t *ctx, ux_instrument_t *instrument,
                       uxe_fundamental_t *fundamental);
void sm_on_execution_report(ux_ctx_t *ctx, uxe_execution_report_t *report);
void sm_on_account_report(ux_ctx_t *ctx, uxe_account_report_t *report);
void sm_on_account_data(ux_ctx_t *ctx, uxe_account_data_t *accountData);
void sm_on_send_order(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_pending_new_order(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_new_order(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_status_changed(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_filled(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_partially_filled(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_cancelled(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_replaced(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_rejected(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_expired(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_cancel_rejected(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_replace_rejected(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_order_done(ux_ctx_t *ctx, ux_order_t *order);
void sm_on_fill(ux_ctx_t *ctx, ux_fill_t *fill);
void sm_on_transaction(ux_ctx_t *ctx, ux_transaction_t *transaction);
void sm_on_position_opened(ux_ctx_t *ctx, ux_position_t *position);
void sm_on_position_closed(ux_ctx_t *ctx, ux_position_t *position);
void sm_on_position_changed(ux_ctx_t *ctx, ux_position_t *position);
void sm_on_stop_executed(ux_ctx_t *ctx, ux_stop_t *stop);
void sm_on_stop_cancelled(ux_ctx_t *ctx, ux_stop_t *stop);
void sm_on_stop_status_changed(ux_ctx_t *ctx, ux_stop_t *stop);
void sm_on_command(ux_ctx_t *ctx, uxe_command_t *command);
void sm_on_user_command(ux_ctx_t *ctx, const char *command);
void sm_on_user_event(ux_ctx_t *ctx, ux_event_t *e);
/*
void sm_on_parameter_changed(ux_ctx_t *ctx,Parameter oldParameter, Parameter
newParameter); void sm_on_property_changed(ux_ctx_t *ctx,OnPropertyChanged
onPropertyChanged);
*/

#ifdef __cplusplus
}
#endif

#endif // __UX_INTERNAL_H__
