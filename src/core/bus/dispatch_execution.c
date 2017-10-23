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

void event_execution_report_dispatch(ux_ctx_t *ctx, ux_event_t *e)
{
    uxe_execution_report_t *report = (uxe_execution_report_t*)e;

    /*
      if (!executionReport.isLoaded) {
         if (this.framework.Clock.Mode == ClockMode.Realtime)
          {
             executionReport.dateTime = this.framework.Clock.DateTime;
          }
          framework.orderManager.on_execution_report(executionReport);
          framework.portfolioManager.OnExecutionReport(executionReport);
          framework.strategyManager.OnExecutionReport(executionReport);
          framework.strategyManager__0.OnExecutionReport(executionReport);
          framework.eventServer.EmitQueued();
          return;
       }
       this.framework.orderManager.OnExecutionReport(executionReport);
       if (executionReport.order != null && executionReport.order.portfolio != null)
       {
          this.framework.portfolioManager.OnExecutionReport(executionReport);
       }
       framework.strategyManager.OnExecutionReport(executionReport);
       framework.strategyManager__0.OnExecutionReport(executionReport);
       framework.eventServer.EmitQueued();
     */
}
