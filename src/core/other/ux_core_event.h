/******************************************************************************
 * Quantitative Kit Library                                                   *
 *                                                                            *
 * Copyright (C) 2017 Xiaojun Gao                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License.    *
 ******************************************************************************/
#ifndef UX_CORE_EVENT_H
#define UX_CORE_EVENT_H

#define CORE_EVENT_DEF(_) \
    _(FrameworkCleared) \
    _(InstrumentAdded) \
    _(InstrumentDeleted) \
    _(InstrumentDefinition) \
    _(InstrumentDefinitionEnd) \
    _(ProviderAdded) \
    _(ProviderRemoved) \
    _(ProviderStatusChanged) \
    _(ProviderConnected) \
    _(ProviderDisconnected) \
    _(ExecutionCommand) \
    _(ExecutionReport) \
    _(AccountReport) \
    _(OrderManagerCleared) \
    _(Fill) \
    _(Transaction) \
    _(Bar) \
    _(BarOpen) \
    _(Bid) \
    _(Ask) \
    _(Trade) \
    _(ProviderError) \
    _(HistoricalData) \
    _(HistoricalDataEnd) \
    _(PortfolioAdded) \
    _(PortfolioRemoved) \
    _(StrategyAdded) \
    _(PositionOpened) \
    _(PositionChanged) \
    _(PositionClosed) \
    _(PortfolioParentChanged) \
    _(NewGroup) \
    _(NewGroupEvent) \
    _(NewGroupUpdate) \
    _(SimulatorProgress) \
    _(SimulatorStop) \
    _(Command) \
    _(Response) \
    _(Output) \
    _(AccountData) \
    _(EventManagerStarted) \
    _(EventManagerStopped) \
    _(EventManagerPaused) \
    _(EventManagerResumed) \
    _(EventManagerStep) \
    _(OptimizationStart) \
    _(OptimizationStop) \
    _(OptimizationProgress) \

#define EVENT_CUSTOM_START 250

typedef enum {
    Event = 0,
    Tick = 1,
    Quote = 5,                      //      ++

    Command = 54,                   //xx    ++  ss
    Response = 55,                  //xx

    /*ResponseEvent = 60,*/

    /*------------------------------------*/
    /*OnExecutionCommand = 115,
    OnExecutionReport = 116,*/

    OnStrategyEvent = 141,          //..    ++

    OnStrategyAdded = 144,          //xx
    OnPropertyChanged = 145,        //..    ++  ss


    OnConnect = 170,
    OnDisconnect = 171,
    OnSubscribe = 172,
    OnUnsubscribe = 173,

    OnFrameworkCleared = 99,        //xx
    OnOrderManagerCleared = 130,    //xx

    OnEventManagerStarted = 176,    //xx
    OnEventManagerStopped = 177,    //xx
    OnEventManagerPaused = 178,     //xx
    OnEventManagerResumed = 179,    //xx
    OnEventManagerStep = 180,       //xx


}EventType;

enum process_ux{
    Bid = 2,                        //xx    ++  ss
    Ask = 3,                        //xx    ++  ss
    Trade = 4,                      //xx    ++  ss

    Bar = 6,                        // baropen , bar    ++  ss
    BarSlice = 140,                 //..    ++  ss
    //Level2 = 7,
    Level2Snapshot = 8,             //      ++  ss
    Level2Update = 9,               //      ++  ss
    Fundamental = 22,               //      ++  ss
    News = 23,                      //      ++  ss


    HistoricalData = 138,           //xx    ++
    HistoricalDataEnd = 139,        //xx    ++

    OnInstrumentDefinition = 131,   //xx
    OnInstrumentDefintionEnd = 132, //xx

    // instrument
    OnInstrumentAdded = 190,        //xx
    OnInstrumentDeleted = 191,      //xx


    AccountData = 142,              //xx    ++  ss
    ExecutionReport = 13,           //xx    ++  ss
    AccountReport = 164,            //xx    ++  ss
    ExecutionCommand = 14,          //xx

    OnFill = 113,                   //xx    ++  ss
    OnTransaction = 114,            //xx    ++  ss

    OnSendOrder = 117,              //      ++
    OnPendingNewOrder = 118,        //      ++
    OnNewOrder = 119,               //      ++
    OnOrderStatusChanged = 120,     //      ++
    OnOrderPartiallyFilled = 121,   //      ++
    OnOrderFilled = 122,            //      ++
    OnOrderReplaced = 123,          //      ++
    OnOrderCancelled = 124,         //      ++
    OnOrderRejected = 125,          //      ++
    OnOrderExpired = 126,           //      ++
    OnOrderCancelRejected = 127,    //      ++
    OnOrderReplaceRejected = 128,   //      ++
    OnOrderDone = 129,              //      ++

    // provider
    OnProviderAdded = 192,          //xx
    OnProviderRemoved = 193,        //xx
    OnProviderConnected = 194,      //xx    ++  ss
    OnProviderDisconnected = 195,   //xx    ++  ss
    OnProviderStatusChanged = 196,  //xx
    ProviderError = 21,             //xx    ++  ss

    OnPortfolioAdded = 135,         //xx    ++
    OnPortfolioRemoved = 136,       //xx    ++
    OnPortfolioParentChanged = 137, //xx    ++
    PortfolioUpdate = 28,

    OnPositionOpened = 110,         //xx    ++  ss
    OnPositionClosed = 111,         //xx    ++  ss
    OnPositionChanged = 112,        //xx    ++  ss

    OnQueueOpened = 174,
    OnQueueClosed = 175,

    // simulator
    OnSimulatorStart = 197,         //..    ++
    OnSimulatorStop = 198,          //xx    ++
    OnSimulatorProgress = 199,      //xx    ++

    OnOptimizationStart = 147,      //xx    ++
    OnOptimizationStop = 148,       //xx    ++
    OnOptimizationProgress = 149,   //xx    ++

    OnUserCommand = 181,            //..    ++  ss

    OnException = 163,              //      ++  ss

    Reminder = 15,                  //      ++

    Output = 27,                    //xx
    /*------------------------------------*/

    Group = 50,                     //xx    ++
    GroupUpdate = 51,               //xx
    GroupEvent = 52,                //xx    ++
};
void on_event(ux_event_t *e)
{
    if(event_manager_status == PAUSED && stepping &&
            (stepAtTypeId == 0 || stepStopAtTypeId == e.TypeId))
    {
        stepping = false;
    }

    event_count++;

    if (!enabled[e.TypeId])
        return;

    if (event_filter != NULL) {
        if (event_filter->filter(e) == NULL)
            return;
    }

    if (e.TypeId > 250)
        forward_event_to_strategy(e);

    if (event_handles[e.TypeId] != NULL) {
        event_handles[e.TypeId](e);
    }


    if(event_dispatcher != NULL)
        event_dispatcher(e);

    if(event_logger != NULL)
        event_logger(e);
}

#endif // UX_CORE_EVENT_H
