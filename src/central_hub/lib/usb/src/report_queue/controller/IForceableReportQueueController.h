#pragma once

#include "IReportQueueController.h"

class IForceableReportQueueController: public IReportQueueController {
    public:
    /**
     * force a report to be sent over usb.
     *  false return indicates that the report could not be forced into the queue,
     *      and the report will be dropped
     *  true return indicates success
    */
   virtual bool forceReport(report_t &report) const = 0;
};