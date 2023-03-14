#pragma once

#include "payload_types.h"
#include "IReportQueue.h"

class ReportQueueController {
    private:
    const IReportQueue *queue;

    public:
    /**
     * construct a USB controller for the given report queue
    */
    ReportQueueController(const IReportQueue *_queue);

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