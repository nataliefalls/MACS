#pragma once

#include "IReportQueueHandler.h"
#include "IReportQueueController.h"

class ReportQueueFactory {
    public:
    /**
     * Get a controller for a report queue
    */
    static IReportQueueController* getController();

    /**
     * Get a handler for a report queue
    */
    static IReportQueueHandler* getHandler();
};