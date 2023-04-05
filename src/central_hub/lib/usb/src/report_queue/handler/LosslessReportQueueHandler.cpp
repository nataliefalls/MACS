#include "LosslessReportQueueHandler.h"
#include "tusb.h"

LosslessReportQueueHandler::LosslessReportQueueHandler(const IReportQueue *_queue) {
    this->queue = _queue;
}

send_report_status_t LosslessReportQueueHandler::sendNextReport(report_t &report) const {
    bool success = this->queue->peek(&report);
    if (!success) return E_QUEUE_EMPTY;

    success = this->send_report(report);
    if (!success) {
        return E_USB_TRANSFER_FAILED;
    } else {
        // the report successfully sent, so we need to remove the top entry from the queue
        this->queue->pop(&report);
        return SEND_SUCCESS;
    }
}