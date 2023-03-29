#include "ForceableReportQueueController.h"
#include "ReportQueueController.h"

ForceableReportQueueController::ForceableReportQueueController(IForceableReportQueue *_reportQueue, IReportQueueController *_controller):
    reportQueue(_reportQueue), controller(_controller) {}

ForceableReportQueueController::ForceableReportQueueController(IForceableReportQueue *_reportQueue):
    ForceableReportQueueController(_reportQueue, new ReportQueueController(_reportQueue)) {}

ForceableReportQueueController::~ForceableReportQueueController() {
    delete this->reportQueue;
    delete this->controller;
}

bool ForceableReportQueueController::forceReport(report_t &report) const {
    return this->reportQueue->forcePush(report);
}

bool ForceableReportQueueController::report(report_t &report) const {
    return this->controller->report(report);
}

bool ForceableReportQueueController::inputReport(uint8_t moduleID, report_id_t reportID, const payload_t &payload) const {
    return this->controller->inputReport(moduleID, reportID, payload);
}

bool ForceableReportQueueController::moduleConnectedReport(uint8_t moduleID, const module_coordinates_t &coordinates) const {
    return this->controller->moduleConnectedReport(moduleID, coordinates);
}

bool ForceableReportQueueController::moduleDisconnectedReport(uint8_t moduleID) const {
    return this->controller->moduleDisconnectedReport(moduleID);
}