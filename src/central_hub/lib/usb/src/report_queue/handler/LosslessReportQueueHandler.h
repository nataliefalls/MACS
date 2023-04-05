#pragma once

#include "ReportQueueHandler.h"

/**
 * a class that handles reports from a report queue
 * 
 * this class guarantess that if a report could not be successfully
 * sent over USB, then it will not be removed from the report queue
*/
class LosslessReportQueueHandler: public ReportQueueHandler {
    public:
    /**
     * Construct a handler for the given report queue
    */
    LosslessReportQueueHandler(const IReportQueue *_queue);

    LosslessReportQueueHandler() = default;
    
    virtual send_report_status_t sendNextReport(report_t &report) const;
};