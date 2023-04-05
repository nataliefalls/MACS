#include "ReportQueueHandler.h"
#include "tusb.h"

ReportQueueHandler::ReportQueueHandler(const IReportQueue *_inputQueue, const IReportQueue *_connectionQueue):
    inputQueue(_inputQueue), connectionQueue(_connectionQueue) {}

send_report_status_t ReportQueueHandler::sendNextReport() const {
    report_t nextReport;
    bool success = this->getNextReport(&nextReport);

    if (!success) return E_QUEUE_EMPTY;

    success = this->sendReport(nextReport);

    if (!success) return E_USB_TRANSFER_FAILED;

    return SEND_SUCCESS;
}

bool ReportQueueHandler::getNextReport(report_t *report) const {
    // we want to alternate between processing connect / disconnect reports
    // and input reports. this static variable helps keep track of which
    // queue we're prioritizing
    static bool prioritizeConnectionQueue = true;
    const IReportQueue *firstQueue = prioritizeConnectionQueue ? this->connectionQueue : this->inputQueue;
    const IReportQueue *secondQueue = prioritizeConnectionQueue ? this->inputQueue : this->connectionQueue;

    if (firstQueue->queuePeek(report)) {
        prioritizeConnectionQueue = !prioritizeConnectionQueue;
        return true;
    } else if (secondQueue->queuePeek(report)) {
        return true;
    } else {
        return false;
    }
}

bool ReportQueueHandler::sendReport(report_t &report) const {
    if ( !tud_hid_ready() ) return false;
    switch (report.reportID) {
        case REPORT_ID_MODULE_CONNECTED:
            return this->sendModuleConnectedReport(report);
        case REPORT_ID_MODULE_DISCONNECTED:
            return this->sendModuleDisconnectedReport(report);
        case REPORT_ID_BUTTON_DATA: 
            return this->sendButtonReport(report);
        case REPORT_ID_DPAD_DATA:
            return this->sendDpadReport(report);
        case REPORT_ID_JOYSTICK_DATA:
            return this->sendJoystickReport(report);
        default: return false;
    }
}

bool ReportQueueHandler::sendModuleConnectedReport(report_t &report) const {
    module_connected_report_t connected = {
        .moduleID = report.moduleID,
        .coordinates = report.payload.coordinates
    };
    if (tud_hid_report(REPORT_ID_MODULE_CONNECTED, (void *) (&connected), sizeof(connected))) {
        // report was successfully sent, so we can pop it off of the connection queue
        this->connectionQueue->queuePop(&report);
        return true;
    }
    return false;
}

bool ReportQueueHandler::sendModuleDisconnectedReport(report_t &report) const {
    module_disconnected_report_t disconnected = {
        .moduleID = report.moduleID
    };
    if (tud_hid_report(REPORT_ID_MODULE_DISCONNECTED, (void *) (&disconnected), sizeof(disconnected))) {
        // report was successfully sent, so we can pop it off of the connection queue
        this->connectionQueue->queuePop(&report);
        return true;
    }
    return false;
}

bool ReportQueueHandler::sendButtonReport(report_t &report) const {
    button_report_t button = {
        .moduleID = report.moduleID,
        .button = report.payload.button
    };
    if (tud_hid_report(REPORT_ID_BUTTON_DATA, (void *) (&button), sizeof(button))) {
        // report was successfully sent, so we can pop it off of the input queue
        this->inputQueue->queuePop(&report);
        return true;
    }
    return false;
}

bool ReportQueueHandler::sendDpadReport(report_t &report) const {
    dpad_report_t dpad = {
        .moduleID = report.moduleID,
        .dpad = report.payload.dpad
    };
    if (tud_hid_report(REPORT_ID_DPAD_DATA, (void *) (&dpad), sizeof(dpad))) {
        // report was successfully sent, so we can pop it off of the input queue
        this->inputQueue->queuePop(&report);
        return true;
    }
    return false;
}

bool ReportQueueHandler::sendJoystickReport(report_t &report) const {
    joystick_report_t joystick = {
        .moduleID = report.moduleID,
        .joystick = report.payload.joystick
    };
    if (tud_hid_report(REPORT_ID_JOYSTICK_DATA, (void *) (&joystick), sizeof(joystick))) {
        // report was successfully sent, so we can pop it off of the input queue
        this->inputQueue->queuePop(&report);
        return true;
    }
    return false;
}