#pragma once

#include "payload_types.h"
#include "IReportQueue.h"

class ReportQueueController {
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

    ReportQueueController() = default;

    /**
     *  prepare an input report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    bool inputReport(uint8_t moduleID, report_id_t reportID, const payload_t &payload) const;

    /**
     *  prepare a module connected report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    bool moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const;

    /**
     *  prepare a module disconnected report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    bool moduleDisconnectedReport(uint8_t moduleID) const;
};