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
    const IReportQueue *queue;

    public:
    /**
     * Construct a handler for the given report queue
    */
    ReportQueueHandler(const IReportQueue *_queue);

    ReportQueueHandler() = default;
    
    /**
     * send the next report in the report queue over usb.
     * in the case of a E_USB_TRANSFER_FAILED, the missing report
     * will be contained in the given report.
    */
    send_report_status_t sendNextReport(report_t &report) const;

    private:
    bool send_report(const report_t &report) const;
    bool send_module_connected_report(const report_t &report) const;
    bool send_module_disconnected_report(const report_t &report) const;
    bool send_button_report(const report_t &report) const;
    bool send_dpad_report(const report_t &report) const;
    bool send_joystick_report(const report_t &report) const;
};