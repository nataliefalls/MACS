#pragma once

#include "ReportQueueHandler.h"
#include "IForceableReportQueueController.h"

/**
 * Puts a report on the report queue for the demo.
 * Return value indicates whether a report was queued successfully.
*/
bool queue_demo_report(IForceableReportQueueController *controller);