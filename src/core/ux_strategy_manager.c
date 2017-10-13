/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_internal.h"

typedef enum {
    UX_STRATEGY_STATUS_TYPE_STARTED,
    UX_STRATEGY_STATUS_TYPE_STOPPED
}ux_strategy_status_type;

typedef struct ux_strategy_status_info_s {
    ux_strategy_status_type type;
    char *solution;
    char *mode;
}ux_strategy_status_info_t;

typedef struct ux_strategy_manager_s ux_strategy_manager_t;

typedef enum {
    UX_STRATEGY_PERSISTENCE_NONE,
    UX_STRATEGY_PERSISTENCE_FULL,
    UX_STRATEGY_PERSISTENCE_SAVE,
    UX_STRATEGY_PERSISTENCE_LOAD
}ux_strategy_persistence;

struct ux_strategy_manager_s {
    ux_ctx_t *ctx;
    ux_strategy_mode strategy_mode;
    ux_strategy_status strategy_status;
    ux_strategy_persistence strategy_persistence;
    int next_id;
    ux_strategy_t *strategy;
};

void ux_strategy_manager_register_market_data_request(
        ux_data_provider_t *provider, ux_instrument_t *instrument);

void ux_strategy_manager_unregister_market_data_request(
        ux_data_provider_t *provider, ux_instrument_t *instrument);

void ux_strategy_manager_start_strategy(ux_strategy_manager_t *manager, ux_strategy_t *strategy,ux_strategy_mode mode)
{
    manager->strategy = strategy;
    manager->strategy_mode = mode;
   if(manager->ctx->mode == UX_BUS_SIMULATION) {
       //bus_clear();
       //bus_set_time(ctx->data_simulator.start);
   }

    /*
 lock (this)
            {
                Console.WriteLine(DateTime.Now + " StrategyManager::StartStrategy " + strategy.Name);
                this.instrumentsByDataProviderId.Clear();
                this.strategy = strategy;
                this.Mode = mode;
                if (this.framework.Mode == FrameworkMode.Simulation)
                {
                    this.framework.clock.Clear();
                    this.framework.exchangeClock.Clear();
                    this.framework.clock.DateTime = this.framework.ProviderManager.DataSimulator.DateTime1;
                }
                if (this.framework.eventManager.eventManagerStatus != EventManagerStatus.Running)
                {
                    this.framework.eventManager.Start();
                }
                this.EmitGroupEvent(StrategyStatusType.Started);
                if (this.strategyPersistence != StrategyPersistence.Full)
                {
                    if (this.strategyPersistence != StrategyPersistence.Save)
                    {
                        this.framework.orderManager.isPersistent = false;
                        goto IL_10D;
                    }
                }
                this.framework.orderServer.SeriesName = strategy.name;
                this.framework.orderManager.isPersistent = true;
                IL_10D:
                if (this.strategyPersistence == StrategyPersistence.Full ||
                    this.strategyPersistence == StrategyPersistence.Load)
                {
                    this.framework.portfolioManager.Load(strategy.name);
                    this.framework.orderManager.Load(strategy.name, -1);
                }
                strategy.Init();
                if ((this.strategyPersistence == StrategyPersistence.Full ||
                     this.strategyPersistence == StrategyPersistence.Save) && !strategy.portfolio.isLoaded)
                {
                    this.framework.portfolioManager.Save(strategy.portfolio);
                }
                strategy->start();
                if (!this.framework.IsExternalDataQueue)
                {
                    Dictionary<IDataProvider, InstrumentList> dictionary =
                        new Dictionary<IDataProvider, InstrumentList>();
                    while (this.instrumentsByDataProviderId.Count != 0)
                    {
                        IDictionary<IDataProvider, InstrumentList> arg_1BF_0 =
                            new Dictionary<IDataProvider, InstrumentList>(this.instrumentsByDataProviderId);
                        this.instrumentsByDataProviderId.Clear();
                        foreach (KeyValuePair<IDataProvider, InstrumentList> current in
                            new Dictionary<IDataProvider, InstrumentList>(arg_1BF_0))
                        {
                            InstrumentList instrumentList = null;
                            if (!dictionary.TryGetValue(current.Key, out instrumentList))
                            {
                                instrumentList = new InstrumentList();
                                dictionary[current.Key] = instrumentList;
                            }
                            InstrumentList instrumentList2 = new InstrumentList();
                            foreach (Instrument current2 in current.Value)
                            {
                                if (!instrumentList.Contains(current2))
                                {
                                    instrumentList.Add(current2);
                                    instrumentList2.Add(current2);
                                }
                            }
                            if (current.Key is SellSideStrategy && this.framework.SubscriptionManager != null)
                            {
                                this.framework.SubscriptionManager.Subscribe(current.Key, instrumentList2);
                            }
                        }
                    }
                    this.method_4();
                    this.strategyStatus = StrategyStatus.Running;
                    this.instrumentsByDataProviderId = dictionary;
                    if (this.instrumentsByDataProviderId.Count == 0 && mode == StrategyMode.Backtest)
                    {
                        Console.WriteLine(string.Concat(new object[]
                        {
                            DateTime.Now,
                            " StrategyManager::StartStrategy ",
                            strategy.Name,
                            " has no data requests in backtest mode, stopping..."
                        }));
                        this.method_7();
                    }
                    else
                    {
                        foreach (KeyValuePair<IDataProvider, InstrumentList> current3 in this.instrumentsByDataProviderId)
                        {
                            if (!(current3.Key is SellSideStrategy) && this.framework.SubscriptionManager != null)
                            {
                                this.framework.SubscriptionManager.Subscribe(current3.Key, current3.Value);
                            }
                        }
                        if (mode != StrategyMode.Backtest && strategy.FundamentalProvider != null)
                        {
                            strategy.FundamentalProvider.Connect();
                        }
                    }
                }
                else
                {
                    this.method_4();
                    this.strategyStatus = StrategyStatus.Running;
                }
            }
            */


}

ux_strategy_status_info_t *ux_strategy_manager_get_status_info();

void ux_strategy_manager_on_strategy_event(ux_strategy_manager_t *manager,void *data);
void ux_strategy_manager_on_exception(ux_strategy_manager_t *manager,const char *source, ux_event_t *e, ux_event_exception_t *exception);
void ux_strategy_manager_on_instrument_added(ux_strategy_manager_t *manager,ux_instrument_t *instrument);
void ux_strategy_manager_on_instrument_removed(ux_strategy_manager_t *manager,ux_instrument_t *instrument);
void ux_strategy_manager_on_reminder(ux_strategy_manager_t *manager,ux_time_t dateTime, void *data);
void ux_strategy_manager_on_exchange_reminder(ux_strategy_manager_t *manager,ux_time_t dateTime, void *data);
void ux_strategy_manager_on_provider_connected(ux_strategy_manager_t *manager,ux_provider_t *provider);
void ux_strategy_manager_on_provider_disconnected(ux_strategy_manager_t *manager,ux_provider_t *provider);
void ux_strategy_manager_on_provider_error(ux_strategy_manager_t *manager,ux_provider_error_t *error);
void ux_strategy_manager_on_bid(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_bid_t *bid);
void ux_strategy_manager_on_ask(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_ask_t *ask);
void ux_strategy_manager_on_trade(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_trade_t *trade);
void ux_strategy_manager_on_level2_snapshot(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_l2snapshot_t *snapshot);
void ux_strategy_manager_on_Level2_update(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_l2update_t *update);
void ux_strategy_manager_on_bar_open(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_bar_t *bar);
void ux_strategy_manager_on_bar(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_bar_t *bar);
void ux_strategy_manager_on_bar_slice(ux_strategy_manager_t *manager,ux_event_bar_slice_t *slice);
void ux_strategy_manager_on_news(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_news_t *news);
void ux_strategy_manager_on_fundamental(ux_strategy_manager_t *manager,ux_instrument_t *instrument, ux_event_fundamental *fundamental);
void ux_strategy_manager_on_execution_report(ux_strategy_manager_t *manager,ux_event_execution_report_t *report);
void ux_strategy_manager_on_account_report(ux_strategy_manager_t *manager,ux_event_account_report_t *report);
void ux_strategy_manager_on_send_order(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_pending_new_order(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_new_order(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_status_changed(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_filled(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_partially_filled(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_cancelled(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_replaced(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_rejected(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_expired(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_cancel_rejected(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_replace_rejected(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_order_done(ux_strategy_manager_t *manager,ux_order_t *order);
void ux_strategy_manager_on_fill(ux_strategy_manager_t *manager,ux_fill_t *fill);
void ux_strategy_manager_on_transaction(ux_strategy_manager_t *manager,ux_transaction_t *transaction);
void ux_strategy_manager_on_position_opened(ux_strategy_manager_t *manager,ux_position_t *position);
void ux_strategy_manager_on_position_closed(ux_strategy_manager_t *manager,ux_position_t *position);
void ux_strategy_manager_on_position_changed(ux_strategy_manager_t *manager,ux_position_t *position);
void ux_strategy_manager_on_stop_executed(ux_strategy_manager_t *manager,ux_stop_t *stop);
void ux_strategy_manager_on_stop_cancelled(ux_strategy_manager_t *manager,ux_stop_t *stop);
void ux_strategy_manager_on_stop_status_changed(ux_strategy_manager_t *manager,ux_stop_t *stop);
void ux_strategy_manager_on_command(ux_strategy_manager_t *manager,ux_command_t *command);
void ux_strategy_manager_on_user_command(ux_strategy_manager_t *manager,const char *command);
void ux_strategy_manager_on_user_event(ux_strategy_manager_t *manager,ux_event_t *e);
void ux_strategy_manager_on_parameter_changed(ux_strategy_manager_t *manager,Parameter oldParameter, Parameter newParameter);
void ux_strategy_manager_on_account_data(ux_strategy_manager_t *manager,AccountData accountData);
void ux_strategy_manager_on_property_changed(ux_strategy_manager_t *manager,OnPropertyChanged onPropertyChanged);
