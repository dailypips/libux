/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#include "ux_internal.h"


#define STRATEGY_MANAGER_FIELDS                     \
    ux_strategy_mode strategy_mode;                 \
    ux_strategy_status strategy_status;             \
    ux_strategy_persistence strategy_persistence;   \
    int next_id;                                    \
    ux_strategy_t *strategy;

void ux_strategy_manager_register_market_data_request(
        ux_data_provider_t *provider, ux_instrument_t *instrument);

void ux_strategy_manager_unregister_market_data_request(
        ux_data_provider_t *provider, ux_instrument_t *instrument);

void ux_strategy_manager_start_strategy(ux_ctx_t *ctx, ux_strategy_t *strategy,ux_strategy_mode mode)
{
    ctx->strategy = strategy;
    ctx->strategy_mode = mode;
   if(ctx->mode == UX_BUS_SIMULATION) {
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

void ux_strategy_manager_on_strategy_event(ux_ctx_t *ctx,void *data);
void ux_strategy_manager_on_exception(ux_ctx_t *ctx,const char *source, ux_event_t *e, uxe_exception_t *exception);
void ux_strategy_manager_on_instrument_added(ux_ctx_t *ctx,ux_instrument_t *instrument);
void ux_strategy_manager_on_instrument_removed(ux_ctx_t *ctx,ux_instrument_t *instrument);
void ux_strategy_manager_on_reminder(ux_ctx_t *ctx,ux_time_t dateTime, void *data);
void ux_strategy_manager_on_exchange_reminder(ux_ctx_t *ctx,ux_time_t dateTime, void *data);
void ux_strategy_manager_on_provider_connected(ux_ctx_t *ctx,ux_provider_t *provider);
void ux_strategy_manager_on_provider_disconnected(ux_ctx_t *ctx,ux_provider_t *provider);
void ux_strategy_manager_on_provider_error(ux_ctx_t *ctx,ux_provider_error_t *error);
void ux_strategy_manager_on_bid(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_bid_t *bid);
void ux_strategy_manager_on_ask(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_ask_t *ask);
void ux_strategy_manager_on_trade(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_trade_t *trade);
void ux_strategy_manager_on_level2_snapshot(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_level2_snapshot_t *snapshot);
void ux_strategy_manager_on_Level2_update(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_level2_update_t *update);
void ux_strategy_manager_on_bar_open(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_bar_t *bar);
void ux_strategy_manager_on_bar(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_bar_t *bar);
void ux_strategy_manager_on_bar_slice(ux_ctx_t *ctx,uxe_bar_slice_t *slice);
void ux_strategy_manager_on_news(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_news_t *news);
void ux_strategy_manager_on_fundamental(ux_ctx_t *ctx,ux_instrument_t *instrument, uxe_fundamental *fundamental);
void ux_strategy_manager_on_execution_report(ux_ctx_t *ctx,uxe_execution_report_t *report);
void ux_strategy_manager_on_account_report(ux_ctx_t *ctx,uxe_account_report_t *report);
void ux_strategy_manager_on_send_order(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_pending_new_order(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_new_order(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_status_changed(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_filled(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_partially_filled(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_cancelled(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_replaced(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_rejected(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_expired(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_cancel_rejected(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_replace_rejected(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_order_done(ux_ctx_t *ctx,ux_order_t *order);
void ux_strategy_manager_on_fill(ux_ctx_t *ctx,ux_fill_t *fill);
void ux_strategy_manager_on_transaction(ux_ctx_t *ctx,ux_transaction_t *transaction);
void ux_strategy_manager_on_position_opened(ux_ctx_t *ctx,ux_position_t *position);
void ux_strategy_manager_on_position_closed(ux_ctx_t *ctx,ux_position_t *position);
void ux_strategy_manager_on_position_changed(ux_ctx_t *ctx,ux_position_t *position);
void ux_strategy_manager_on_stop_executed(ux_ctx_t *ctx,ux_stop_t *stop);
void ux_strategy_manager_on_stop_cancelled(ux_ctx_t *ctx,ux_stop_t *stop);
void ux_strategy_manager_on_stop_status_changed(ux_ctx_t *ctx,ux_stop_t *stop);
void ux_strategy_manager_on_command(ux_ctx_t *ctx,ux_command_t *command);
void ux_strategy_manager_on_user_command(ux_ctx_t *ctx,const char *command);
void ux_strategy_manager_on_user_event(ux_ctx_t *ctx,ux_event_t *e);
void ux_strategy_manager_on_parameter_changed(ux_ctx_t *ctx,Parameter oldParameter, Parameter newParameter);
void ux_strategy_manager_on_account_data(ux_ctx_t *ctx,AccountData accountData);
void ux_strategy_manager_on_property_changed(ux_ctx_t *ctx,OnPropertyChanged onPropertyChanged);
