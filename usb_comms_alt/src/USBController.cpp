#include "IReportQueue.h"
#include "USBController.h"

USBController::USBController(const IReportQueue *_queue)  {
    this->queue = _queue;
}

bool USBController::inputReport(const report_t &report) const {
    return this->queue->queue_push(report);
}

bool USBController::moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const {
    report_t newReport = {
        REPORT_ID_MODULE_CONNECTED, // .reportID
        moduleID,                   // .moduleID
        coordinates,                // .payload.coordinates
    };
    return this->queue->queue_push(newReport);
}

bool USBController::moduleDisconnectedReport(uint8_t moduleID) const {
    report_t newReport = {
        REPORT_ID_MODULE_DISCONNECTED,  // .reportID
        moduleID,                       // .moduleID
    };
    return this->queue->queue_push(newReport);
}