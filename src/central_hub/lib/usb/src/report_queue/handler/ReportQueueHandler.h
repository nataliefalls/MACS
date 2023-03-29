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
     * pop a report off of the report queue and send it over usb.
     * 
     * return value indicates the status of the send:
     * - SEND_SUCCESS: the send was successful
     * - E_QUEUE_EMPTY: the queue was empty, so nothing was sent
     * - E_USB_TRANSFER_FAILED: a report was popped off of the queue, but it
     *      could not be sent over USB. In this case, the given report will
     *      contain this report. If nothing is done with the given report,
     *      it will be dropped.
    */
    send_report_status_t sendNextReport(report_t &report) const;
    
    /**
     * pop a report off of the report queue and send it over usb.
     * 
     * return value indicates the status of the send:
     * - SEND_SUCCESS: the send was successful
     * - E_QUEUE_EMPTY: the queue was empty, so nothing was sent
     * - E_USB_TRANSFER_FAILED: a report was popped off of the queue, but it
     *      could not be sent over USB and was dropped
    */
    send_report_status_t sendNextReport() const;

    private:
    bool send_report(const report_t &report) const;
    bool send_module_connected_report(const report_t &report) const;
    bool send_module_disconnected_report(const report_t &report) const;
    bool send_button_report(const report_t &report) const;
    bool send_dpad_report(const report_t &report) const;
    bool send_joystick_report(const report_t &report) const;
};