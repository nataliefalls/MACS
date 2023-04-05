#pragma once

#include "IReportQueueController.h"

class ReportQueueController: public IReportQueueController {
    private:
    const IReportQueue *queue;

    public:
    /**
     * construct a USB controller for the given report queue
    */
    ReportQueueController(const IReportQueue *_queue);

    ReportQueueController() = default;

    ~ReportQueueController();

    virtual bool report(report_t &report) const;

    virtual bool inputReport(uint8_t moduleID, report_id_t reportID, const payload_t &payload) const;

    virtual bool moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const;

    virtual bool moduleDisconnectedReport(uint8_t moduleID) const;
};