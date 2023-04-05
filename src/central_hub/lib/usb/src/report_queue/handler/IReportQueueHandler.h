#pragma once

#include "IReportQueue.h"

/**
 * status codes for send_next_report
*/
enum send_report_status_t {
    /**
     * the send was successful
    */
    SEND_SUCCESS = 0,
    /**
     * the queue was empty, so nothing was sent
    */
    E_QUEUE_EMPTY,
    /**
     * a report was popped off of the queue, but it could not be sent over USB
    */
    E_USB_TRANSFER_FAILED,
};

class IReportQueueHandler {
    public:
    /**
     * pop a report off of the report queue and send it over usb.
     * 
     * if the report queue is non-empty, the given report will contain the top report.
    */
    virtual send_report_status_t sendNextReport(report_t &report) const = 0;
    
    /**
     * pop a report off of the report queue and send it over usb.
    */
    virtual send_report_status_t sendNextReport() const = 0;
};