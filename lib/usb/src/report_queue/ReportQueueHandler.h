#pragma once

#include "IReportQueueHandler.h"
#include "IReportQueue.h"

class ReportQueueHandler: public IReportQueueHandler {
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

    /**
     * do nothing, since the handler doesn't own the queues it's given
    */
    ~ReportQueueHandler();

    virtual send_report_status_t sendNextReport() const;

    private:
    bool getNextReport(report_t *report) const;
    bool sendReport(report_t &report) const;
    bool sendModuleConnectedReport(report_t &report) const;
    bool sendModuleDisconnectedReport(report_t &report) const;
    bool sendDigitalReport(report_t &report) const;
    bool sendAnalogReport(report_t &report) const;
    bool sendJoystickReport(report_t &report) const;
};