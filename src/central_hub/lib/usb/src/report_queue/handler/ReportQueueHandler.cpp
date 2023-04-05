#include "ReportQueueHandler.h"
#include "tusb.h"

ReportQueueHandler::ReportQueueHandler(const IReportQueue *_queue) {
    this->queue = _queue;
}

send_report_status_t ReportQueueHandler::sendNextReport() const {
    report_t nextReport;
    bool success = this->queue->queue_pop(&nextReport);

    if (!success) return E_QUEUE_EMPTY;

    success = this->send_report(nextReport);

    if (!success) return E_USB_TRANSFER_FAILED;

    return SEND_SUCCESS;
}

bool ReportQueueHandler::send_report(const report_t &report) const {
    bool ready = tud_hid_ready();
    if ( !ready ) return false;
    switch (report.reportID) {
        case REPORT_ID_MODULE_CONNECTED:
            return this->send_module_connected_report(report);
        case REPORT_ID_MODULE_DISCONNECTED:
            return this->send_module_disconnected_report(report);
        case REPORT_ID_BUTTON_DATA: 
            return this->send_button_report(report);
        case REPORT_ID_DPAD_DATA:
            return this->send_dpad_report(report);
        case REPORT_ID_JOYSTICK_DATA:
            return this->send_joystick_report(report);
        default: return false;
    }
}

bool ReportQueueHandler::send_module_connected_report(const report_t &report) const {
    module_connected_report_t connected = {
        .moduleID = report.moduleID,
        .coordinates = report.payload.coordinates
    };
    return tud_hid_report(REPORT_ID_MODULE_CONNECTED, (void *) (&connected), sizeof(connected));
}

bool ReportQueueHandler::send_module_disconnected_report(const report_t &report) const {
    module_disconnected_report_t disconnected = {
        .moduleID = report.moduleID
    };
    return tud_hid_report(REPORT_ID_MODULE_DISCONNECTED, (void *) (&disconnected), sizeof(disconnected));
}

bool ReportQueueHandler::send_button_report(const report_t &report) const {
    button_report_t button = {
        .moduleID = report.moduleID,
        .button = report.payload.button
    };
    return tud_hid_report(REPORT_ID_BUTTON_DATA, (void *) (&button), sizeof(button));
}

bool ReportQueueHandler::send_dpad_report(const report_t &report) const {
    dpad_report_t dpad = {
        .moduleID = report.moduleID,
        .dpad = report.payload.dpad
    };
    return tud_hid_report(REPORT_ID_DPAD_DATA, (void *) (&dpad), sizeof(dpad));
}

bool ReportQueueHandler::send_joystick_report(const report_t &report) const {
    joystick_report_t joystick = {
        .moduleID = report.moduleID,
        .joystick = report.payload.joystick
    };
    return tud_hid_report(REPORT_ID_JOYSTICK_DATA, (void *) (&joystick), sizeof(joystick));
}