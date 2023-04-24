#pragma once

#include "IReportQueueController.h"
#include "IReportQueue.h"

class ReportQueueController: public IReportQueueController {
    private:
    const IReportQueue *inputQueue;
    // inputs are less important to process than connect / disconnect messages, so
    // we should have a separate queue for them so that they don't get drowned out
    const IReportQueue *connectionQueue;

    public:
    /**
     * construct a report queue controller for the given report queues
     * first arg is the input queue
     * second arg is the connection queue
    */
    ReportQueueController(const IReportQueue *_inputQueue, const IReportQueue *_connectionQueue);

    /**
     * do nothing, since the controller doesn't own the queues it's given
    */
    ~ReportQueueController();

    virtual bool inputReport(uint8_t moduleID, report_id_t reportID, const payload_t &payload) const;

    virtual bool moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const;

    virtual bool moduleDisconnectedReport(uint8_t moduleID) const;
};