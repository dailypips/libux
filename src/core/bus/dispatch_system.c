/******************************************************************************
 * Automated Trading System                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the MIT License.             *
 ******************************************************************************/

#include "dispatch.h"
#include "context.h"
#include "event_vtable.h"


/* copy */

/*
void OnProviderConnected(Event e)
{
    Provider provider = ((OnProviderConnected) e).provider;
    if (provider is IDataProvider)
    {
        this.framework.SubscriptionManager.ReSubscribe((IDataProvider) provider);
    }
    this.framework.strategyManager.OnProviderConnected(provider);
    this.framework.strategyManager__0.OnProviderConnected(provider);
}

void OnProviderDisconnected(Event e)
{
    Provider provider = ((OnProviderDisconnected) e).provider;
    if (provider is IDataProvider)
    {
        this.framework.SubscriptionManager.DeSubscribe((IDataProvider) provider);
    }
    this.framework.strategyManager.OnProviderDisconnected(provider);
    this.framework.strategyManager__0.OnProviderDisconnected(provider);
}

void OnProviderError(Event e)
{
    this.framework.strategyManager.OnProviderError((ProviderError) e);
    this.framework.strategyManager__0.OnProviderError((ProviderError) e);
}

void OnSimulatorStart(Event e)
{
    OnSimulatorStart onSimulatorStart = (OnSimulatorStart) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = onSimulatorStart.dateTime_1;
    }
    if (this.eventBus != null)
    {
        this.eventBus.ResetCounts();
    }
    this.eventCount = 0L;
    this.dataEventCount = 0L;
    this.stopwatch.Reset();
    this.stopwatch.Start();
}

void OnSimulatorStop(Event e)
{
    this.framework.strategyManager.Stop();
    this.framework.strategyManager__0.Stop();
    this.stopwatch.Stop();
    long elapsedMilliseconds = this.stopwatch.ElapsedMilliseconds;
    if (elapsedMilliseconds != 0L)
    {
        Console.WriteLine(string.Concat(new object[]
        {
            DateTime.Now,
            " Data run done, count = ",
            this.dataEventCount,
            " ms = ",
            this.stopwatch.ElapsedMilliseconds,
            " event/sec = ",
            this.dataEventCount / elapsedMilliseconds * 1000L
        }));
        return;
    }
    Console.WriteLine(string.Concat(new object[]
    {
        DateTime.Now,
        " Data run done, count = ",
        this.dataEventCount,
        " ms = 0"
    }));
}

void OnSimulatorProgress(Event e)
{
}

void OnOptimizationStart(Event e)
{
    this.framework.optimizationManager.OnOptimizationStart();
}

void OnOptimizationStop(Event e)
{
    this.framework.optimizationManager.OnOptimizationStop();
}

void OnOptimizationProgress(Event e)
{
    this.framework.optimizationManager.OnOptimizationProgress();
}

void OnBid(Event e)
{
    this.dataEventCount += 1L;
    Bid bid = (Bid) e;
    if (this.framework.clock.Mode == ClockMode.Simulation)
    {
        this.framework.clock.DateTime = bid.dateTime;
    }
    else
    {
        bid.dateTime = this.framework.clock.DateTime;
    }
    if (bid.exchangeDateTime > this.framework.exchangeClock.DateTime)
    {
        this.framework.exchangeClock.DateTime = bid.exchangeDateTime;
    }
    else if (bid.exchangeDateTime > this.framework.exchangeClock.DateTime)
    {
        Console.WriteLine(string.Concat(new object[]
        {
            "EventManager::OnBid Exchange datetime is out of synch : bid datetime = ",
            bid.exchangeDateTime,
            " clock datetime = ",
            this.framework.exchangeClock.DateTime
        }));
    }
    this.barFactory.OnTick(bid);
    this.framework.dataManager.OnBid(bid);
    this.framework.instrumentManager.GetById(bid.instrumentId).bid = bid;
    this.framework.providerManager.executionSimulator.OnBid(bid);
    this.framework.strategyManager.OnBid(bid);
    this.framework.strategyManager__0.OnBid(bid);
}

void OnAsk(Event e)
{
    this.dataEventCount += 1L;
    Ask ask = (Ask) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = ask.DateTime;
    }
    else
    {
        ask.dateTime = this.framework.clock.DateTime;
    }
    if (ask.exchangeDateTime > this.framework.exchangeClock.DateTime)
    {
        this.framework.exchangeClock.DateTime = ask.exchangeDateTime;
    }
    else if (ask.exchangeDateTime > this.framework.exchangeClock.DateTime)
    {
        Console.WriteLine(string.Concat(new object[]
        {
            "EventManager::OnAsk Exchange datetime is out of synch : ask datetime = ",
            ask.exchangeDateTime,
            " clock datetime = ",
            this.framework.exchangeClock.DateTime
        }));
    }
    this.barFactory.OnTick(ask);
    this.framework.dataManager.OnAsk(ask);
    this.framework.instrumentManager.GetById(ask.instrumentId).ask = ask;
    this.framework.providerManager.executionSimulator.OnAsk(ask);
    this.framework.strategyManager.OnAsk(ask);
    this.framework.strategyManager__0.OnAsk(ask);
}

void OnTrade(Event e)
{
    this.dataEventCount += 1L;
    Trade trade = (Trade) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = trade.dateTime;
    }
    else
    {
        trade.dateTime = this.framework.clock.DateTime;
    }
    if (trade.exchangeDateTime > this.framework.exchangeClock.DateTime)
    {
        this.framework.exchangeClock.DateTime = trade.exchangeDateTime;
    }
    else if (trade.exchangeDateTime > this.framework.exchangeClock.DateTime)
    {
        Console.WriteLine(string.Concat(new object[]
        {
            "EventManager::OnTrade Exchange datetime is out of synch : trade datetime = ",
            trade.exchangeDateTime,
            " clock datetime = ",
            this.framework.exchangeClock.DateTime
        }));
    }
    this.barFactory.OnTick(trade);
    this.framework.dataManager.OnTrade(trade);
    this.framework.instrumentManager.GetById(trade.instrumentId).trade = trade;
    this.framework.providerManager.executionSimulator.OnTrade(trade);
    this.framework.strategyManager.OnTrade(trade);
    this.framework.strategyManager__0.OnTrade(trade);
}

void OnQuote(Event e)
{
    Quote quote = (Quote) e;
    if (quote.bid != null && quote.bid.price != 0.0)
    {
        Bid bid = this.framework.dataManager.GetBid(quote.bid.instrumentId);
        if (bid == null || quote.bid.price != bid.price || quote.bid.size != bid.size)
        {
            this.OnBid(quote.bid);
        }
    }
    if (quote.ask != null && quote.ask.price != 0.0)
    {
        Ask ask = this.framework.dataManager.GetAsk(quote.ask.instrumentId);
        if (ask == null || quote.ask.price != ask.price || quote.ask.size != ask.size)
        {
            this.OnAsk(quote.ask);
        }
    }
}

void OnBar(Event e)
{
    this.dataEventCount += 1L;
    Bar bar = (Bar) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = bar.dateTime;
    }
    if (bar.barStatus != BarStatus.Open)
    {
        this.framework.dataManager.OnBar(bar);
        this.framework.instrumentManager.GetById(bar.instrumentId).bar = bar;
        this.framework.providerManager.executionSimulator.OnBar(bar);
        this.framework.strategyManager.OnBar(bar);
        this.framework.strategyManager__0.OnBar(bar);
        if (bar.barType == BarType.Time || bar.barType == BarType.Session)
        {
            this.barSliceFactory.method_1(bar);
        }
        return;
    }
    if ((bar.barType == BarType.Time || bar.barType == BarType.Session) && !this.barSliceFactory.method_0(bar))
    {
        return;
    }
    this.framework.providerManager.executionSimulator.OnBarOpen(bar);
    this.framework.strategyManager.OnBarOpen(bar);
    this.framework.strategyManager__0.OnBarOpen(bar);
}

void OnBarSlice(Event e)
{
    BarSlice barSlice = (BarSlice) e;
    barSlice.dateTime = this.framework.clock.DateTime;
    this.framework.strategyManager.OnBarSlice(barSlice);
    this.framework.strategyManager__0.OnBarSlice(barSlice);
}

void OnLevel2Snapshot(Event e)
{
    this.dataEventCount += 1L;
    Level2Snapshot level2Snapshot = (Level2Snapshot) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = level2Snapshot.dateTime;
    }
    else
    {
        level2Snapshot.dateTime = this.framework.Clock.DateTime;
    }
    this.framework.dataManager.OnLevel2Snapshot(level2Snapshot);
    this.framework.providerManager.executionSimulator.OnLevel2(level2Snapshot);
    this.framework.strategyManager.OnLevel2Snapshot(level2Snapshot);
    this.framework.strategyManager__0.OnLevel2Snapshot(level2Snapshot);
}

void OnLevel2Update(Event e)
{
    this.dataEventCount += 1L;
    Level2Update level2Update = (Level2Update) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = level2Update.dateTime;
    }
    else
    {
        level2Update.dateTime = this.framework.Clock.DateTime;
    }
    this.framework.dataManager.OnLevel2Update(level2Update);
    this.framework.providerManager.executionSimulator.OnLevel2(level2Update);
    this.framework.strategyManager.OnLevel2Update(level2Update);
    this.framework.strategyManager__0.OnLevel2Update(level2Update);
}

void OnFundamental(Event e)
{
    this.dataEventCount += 1L;
    Fundamental fundamental = (Fundamental) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = fundamental.dateTime;
    }
    else
    {
        fundamental.dateTime = this.framework.clock.DateTime;
    }
    this.framework.dataManager.OnFundamental(fundamental);
    this.framework.strategyManager.OnFundamental(fundamental);
}

void OnNews(Event e)
{
    this.dataEventCount += 1L;
    News news = (News) e;
    if (this.framework.Clock.Mode == ClockMode.Simulation)
    {
        this.framework.Clock.DateTime = news.dateTime;
    }
    else
    {
        news.dateTime = this.framework.clock.DateTime;
    }
    this.framework.dataManager.OnNews(news);
    this.framework.strategyManager.OnNews(news);
}

void OnExecutionReport(Event e)
{
    ExecutionReport executionReport = (ExecutionReport) e;
    if (!executionReport.isLoaded)
    {
        if (this.framework.Clock.Mode == ClockMode.Realtime)
        {
            executionReport.dateTime = this.framework.Clock.DateTime;
        }
        this.framework.orderManager.on_execution_report(executionReport);
        this.framework.portfolioManager.OnExecutionReport(executionReport);
        this.framework.strategyManager.OnExecutionReport(executionReport);
        this.framework.strategyManager__0.OnExecutionReport(executionReport);
        this.framework.eventServer.EmitQueued();
        return;
    }
    this.framework.orderManager.OnExecutionReport(executionReport);
    if (executionReport.order != null && executionReport.order.portfolio != null)
    {
        this.framework.portfolioManager.OnExecutionReport(executionReport);
    }
    this.framework.strategyManager.OnExecutionReport(executionReport);
    this.framework.strategyManager__0.OnExecutionReport(executionReport);
    this.framework.eventServer.EmitQueued();
}

void OnAccountReport(Event e)
{
    AccountReport accountReport_ = (AccountReport) e;
    this.framework.OrderManager.on_account_report(accountReport_);
    this.framework.PortfolioManager.OnAccountReport(accountReport_);
    this.framework.strategyManager.OnAccountReport(accountReport_);
    this.framework.strategyManager__0.OnAccountReport(accountReport_);
}

void OnAccountData(Event e)
{
    AccountData accountData_ = (AccountData) e;
    this.framework.accountDataManager.OnAccountData(accountData_);
    this.framework.strategyManager.OnAccountData(accountData_);
}

void OnSendOrder(Event e)
{
    this.framework.strategyManager.OnSendOrder(((OnSendOrder) e).order_);
    this.framework.strategyManager__0.OnSendOrder(((OnSendOrder) e).order_);
}

void OnPendingNewOrder(Event e)
{
    this.framework.strategyManager.OnPendingNewOrder(((OnPendingNewOrder) e).order_);
    this.framework.strategyManager__0.OnPendingNewOrder(((OnPendingNewOrder) e).order_);
}

void OnNewOrder(Event e)
{
    this.framework.strategyManager.OnNewOrder(((OnNewOrder) e).order_);
    this.framework.strategyManager__0.OnNewOrder(((OnNewOrder) e).order_);
}

void OnOrderStatusChanged(Event e)
{
    this.framework.strategyManager.OnOrderStatusChanged(((OnOrderStatusChanged) e).order_);
    this.framework.strategyManager__0.OnOrderStatusChanged(((OnOrderStatusChanged) e).order_);
}

void OnOrderPartiallyFilled(Event e)
{
    this.framework.strategyManager.OnOrderPartiallyFilled(((OnOrderPartiallyFilled) e).order_0);
    this.framework.strategyManager__0.OnOrderPartiallyFilled(((OnOrderPartiallyFilled) e).order_0);
}

void OnOrderFilled(Event e)
{
    this.framework.strategyManager.OnOrderFilled(((OnOrderFilled) e).order_);
    this.framework.strategyManager__0.OnOrderFilled(((OnOrderFilled) e).order_);
}

void OnOrderReplaced(Event e)
{
    this.framework.strategyManager.method_28(((OnOrderReplaced) e).order_);
    this.framework.strategyManager__0.OnOrderReplaced(((OnOrderReplaced) e).order_);
}

void OnOrderCancelled(Event e)
{
    this.framework.strategyManager.OnOrderCancelled(((OnOrderCancelled) e).order_);
    this.framework.strategyManager__0.OnOrderCancelled(((OnOrderCancelled) e).order_);
}

void OnOrderRejected(Event e)
{
    this.framework.strategyManager.OnOrderRejected(((OnOrderRejected) e).order_);
    this.framework.strategyManager__0.OnOrderRejected(((OnOrderRejected) e).order_);
}

void OnOrderExpired(Event e)
{
    this.framework.strategyManager.OnOrderExpired(((OnOrderExpired) e).order_);
    this.framework.strategyManager__0.OnOrderExpired(((OnOrderExpired) e).order_);
}

void OnOrderCancelRejected(Event e)
{
    this.framework.strategyManager.OnOrderCancelRejected(((OnOrderCancelRejected) e).order_);
    this.framework.strategyManager__0.OnOrderCancelRejected(((OnOrderCancelRejected) e).order_);
}

void OnOrderReplaceRejected(Event e)
{
    this.framework.strategyManager.OnOrderReplaceRejected(((OnOrderReplaceRejected) e).order_);
    this.framework.strategyManager__0.OnOrderReplaceRejected(((OnOrderReplaceRejected) e).order_);
}

void OnOrderDone(Event e)
{
    this.framework.strategyManager.OnOrderDone(((OnOrderDone) e).order_);
    this.framework.strategyManager__0.OnOrderDone(((OnOrderDone) e).order_);
}

void OnFill(Event e)
{
    this.framework.strategyManager.OnFill((OnFill) e);
    this.framework.strategyManager__0.method_28((OnFill) e);
}

void OnTransaction(Event e)
{
    this.framework.strategyManager.OnTransaction((OnTransaction) e);
    this.framework.strategyManager__0.OnTransaction((OnTransaction) e);
}

void OnPositionOpened(Event e)
{
    OnPositionOpened onPositionOpened = (OnPositionOpened) e;
    this.framework.strategyManager.OnPositionOpened(onPositionOpened.portfolio, onPositionOpened.position);
    this.framework.strategyManager__0.OnPositionOpened(onPositionOpened.portfolio, onPositionOpened.position);
}

void OnPositionClosed(Event e)
{
    OnPositionClosed onPositionClosed = (OnPositionClosed) e;
    this.framework.strategyManager.OnPositionClosed(onPositionClosed.portfolio, onPositionClosed.position);
    this.framework.strategyManager__0.OnPositionClosed(onPositionClosed.portfolio, onPositionClosed.position);
}

void OnPositionChanged(Event e)
{
    OnPositionChanged onPositionChanged = (OnPositionChanged) e;
    this.framework.strategyManager.OnPositionChanged(onPositionChanged.portfolio, onPositionChanged.position);
    this.framework.strategyManager__0.OnPositionChanged(onPositionChanged.portfolio, onPositionChanged.position);
}

void OnReminder(Event e)
{
    Reminder reminder = (Reminder) e;
    if ((reminder.clock.clockType == ClockType.Local && reminder.clock.clockMode == ClockMode.Simulation) ||
        reminder.clock.clockType == ClockType.Exchange)
    {
        reminder.clock.DateTime = e.dateTime;
    }
    ((Reminder) e).Execution();
}

void OnGroup(Event e)
{
    this.framework.groupManager.OnGroup((Group) e);
}

void OnGroupEvent(Event e)
{
    this.framework.groupManager.OnGroupEvent((GroupEvent) e);
}

void OnHistoricalData(Event e)
{
    this.framework.dataManager.OnHistoricalData((HistoricalData) e);
}

void OnHistoricalDataEnd(Event e)
{
    this.framework.dataManager.OnHistoricalDataEnd((HistoricalDataEnd) e);
}

void OnCommand(Event e)
{
    this.framework.strategyManager.OnCommand((Command_) e);
    this.framework.strategyManager__0.OnCommand((Command_) e);
}

void OnUserCommand(Event e)
{
    this.framework.strategyManager.OnUserCommand(((OnUserCommand) e).string_0);
    this.framework.strategyManager__0.OnUserCommand(((OnUserCommand) e).string_0);
}

void OnStrategyEvent(Event e)
{
    this.framework.strategyManager.OnStrategyEvent(((OnStrategyEvent) e).object_0);
}

void OnException(Event e)
{
    Console.WriteLine(string.Concat(new object[]
    {
        "EventManager::OnException Exception occured in ",
        ((OnException) e).source_,
        " - ",
        ((OnException) e).event_,
        " - ",
        ((OnException) e).exception_
    }));
    this.framework.strategyManager.method_54(((OnException) e).source_, ((OnException) e).event_,
        ((OnException) e).exception_);
    this.framework.strategyManager__0.method_3(((OnException) e).source_, ((OnException) e).event_,
        ((OnException) e).exception_);
}

void OnPortfolioAdded(Event e)
{
    this.framework.strategyManager.OnPortfolioAdded(((OnPortfolioAdded) e).portfolio_);
}

void OnPortfolioRemoved(Event e)
{
    this.framework.strategyManager.OnPortfolioRemoved(((OnPortfolioRemoved) e).portfolio);
}

void OnPortfolioParentChanged(Event e)
{
    this.framework.strategyManager.OnPortfolioParentChanged(((OnPortfolioParentChanged) e).portfolio);
    this.framework.strategyManager__0.OnPortfolioParentChanged(((OnPortfolioParentChanged) e).portfolio);
}

void OnPropertyChanged(Event e)
{
    this.framework.strategyManager.OnPropertyChanged((OnPropertyChanged) e);
    this.framework.strategyManager__0.OnPropertyChanged((OnPropertyChanged) e);
}

void OnUserEvent(Event e)
{
    this.framework.strategyManager.OnUserEvent(e);
    this.framework.strategyManager__0.OnUserEvent(e);
}
*/
