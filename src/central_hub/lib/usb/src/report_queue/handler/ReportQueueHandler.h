#pragma once

#include "IReportQueue.h"

// error status codes for send_next_report
enum send_report_status_t {
    SEND_SUCCESS = 0,
    E_QUEUE_EMPTY,
    E_USB_TRANSFER_FAILED,
};

class ReportQueueHandler {
    private:
    const IReportQueue *inputQueue;
    // inputs are less important to process than connect / disconnect messages, so
    // we should have a separate queue for them so that they don't get drowned out
    const IReportQueue *connectionQueue;

    public:
    /**
     * Construct a handler for the given report queues
     * first arg is the input queue
     * second arg is the connection queue
    */
    ReportQueueHandler(const IReportQueue *_inputQueue, const IReportQueue *_connectionQueue);

    ReportQueueHandler() = default;
    
    /**
     * send the next report in the report queue over usb
     * 
    */
    send_report_status_t sendNextReport() const;

    private:
    bool getNextReport(report_t *report) const;
    bool sendReport(report_t &report) const;
    bool sendModuleConnectedReport(report_t &report) const;
    bool sendModuleDisconnectedReport(report_t &report) const;
    bool sendDigitalReport(report_t &report) const;
    bool sendAnalogReport(report_t &report) const;
    bool sendJoystickReport(report_t &report) const;
};