#ifndef USB_CONTROLLER_H
#define USB_CONTROLLER_H

#include "payload_types.h"
#include "IReportQueue.h"

class USBController {
    private:
    const IReportQueue *queue;

    public:
    /**
     * construct a USB controller for the given report queue
    */
    USBController(const IReportQueue *_queue);

    USBController() = default;

    /**
     *  prepare an input report to be sent over usb.
     *  false return indicates that the report could not be queued
     *  true return indicates success
    */
    bool inputReport(const report_t &report) const;

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

#endif