#include "IReportQueue.h"
#include "ReportQueueController.h"

ReportQueueController::ReportQueueController(const IReportQueue *_queue): queue(_queue)  {}

bool ReportQueueController::inputReport(uint8_t moduleID, report_id_t reportID, const payload_t &payload) const {
    report_t report = {
        reportID,   // .reportID
        moduleID,   // .moduleID
        payload,    // .payload
    };
    return this->queue->queue_push(report);
}

bool ReportQueueController::moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const {
    report_t newReport = {
        REPORT_ID_MODULE_CONNECTED,     // .reportID
        moduleID,                       // .moduleID
        { .coordinates = coordinates }, // .payload.coordinates
    };
    return this->queue->queue_push(newReport);
}

bool ReportQueueController::moduleDisconnectedReport(uint8_t moduleID) const {
    report_t newReport = {
        REPORT_ID_MODULE_DISCONNECTED,  // .reportID
        moduleID,                       // .moduleID
    };
    return this->queue->queue_push(newReport);
}