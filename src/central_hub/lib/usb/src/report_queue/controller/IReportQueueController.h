#pragma once

#include "payload_types.h"
#include "IReportQueue.h"

class IReportQueueController {
    public:
    /**
     * prepare a report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    virtual bool report(report_t &report) const = 0;

    /**
     *  prepare an input report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    virtual bool inputReport(uint8_t moduleID, report_id_t reportID, const payload_t &payload) const = 0;

    /**
     *  prepare a module connected report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    virtual bool moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const = 0;

    /**
     *  prepare a module disconnected report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    virtual bool moduleDisconnectedReport(uint8_t moduleID) const = 0;
};