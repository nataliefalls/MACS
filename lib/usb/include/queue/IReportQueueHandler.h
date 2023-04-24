#pragma once

// error status codes for send_next_report
enum send_report_status_t {
    SEND_SUCCESS = 0,
    E_QUEUE_EMPTY,
    E_USB_TRANSFER_FAILED,
};

class IReportQueueHandler {
    public:
    /**
     * send the next report in the report queue over usb
    */
    virtual send_report_status_t sendNextReport() const = 0;
};