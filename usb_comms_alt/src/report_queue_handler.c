#include "report_queue_handler.h"
#include "report_queue.h"
#include "report_types.h"
#include "tusb.h"

bool send_report(report_t report);
bool send_module_connected_report(report_t report);
bool send_module_disconnected_report(report_t report);
bool send_button_report(report_t report);
bool send_dpad_report(report_t report);
bool send_joystick_report(report_t report);

bool send_next_report() {
    report_t nextReport;

    if (queue_pop(&nextReport)) {
        return send_report(nextReport);
    } else {
        return false;
    }
}

bool send_report(report_t report) {
    switch (report.reportID) {
        case REPORT_ID_MODULE_CONNECTED:
            return send_module_connected_report(report);
        case REPORT_ID_MODULE_DISCONNECTED:
            return send_module_disconnected_report(report);
        case REPORT_ID_BUTTON_DATA: 
            return send_button_report(report);
        case REPORT_ID_DPAD_DATA:
            return send_dpad_report(report);
        case REPORT_ID_JOYSTICK_DATA:
            return send_joystick_report(report);
        default: return false;
    }
}

bool send_module_connected_report(report_t report) {
    module_connected_report_t connected = {
        .moduleID = report.moduleID,
        .coordinates = report.payload.coordinates
    };
    return tud_hid_report(REPORT_ID_MODULE_CONNECTED, (void *) (&connected), sizeof(connected));
}

bool send_module_disconnected_report(report_t report) {
    module_disconnected_report_t disconnected = {
        .moduleID = report.moduleID
    };
    return tud_hid_report(REPORT_ID_MODULE_DISCONNECTED, (void *) (&disconnected), sizeof(report));
}

bool send_button_report(report_t report) {
    button_report_t button = {
        .moduleID = report.moduleID,
        .button = report.payload.button
    };
    return tud_hid_report(REPORT_ID_BUTTON_DATA, (void *) (&button), sizeof(report));
}

bool send_dpad_report(report_t report) {
    dpad_report_t dpad = {
        .moduleID = report.moduleID,
        .dpad = report.payload.dpad
    };
    return tud_hid_report(REPORT_ID_DPAD_DATA, (void *) (&dpad), sizeof(report));
}

bool send_joystick_report(report_t report) {
    joystick_report_t joystick = {
        .moduleID = report.moduleID,
        .joystick = report.payload.joystick
    };
    return tud_hid_report(REPORT_ID_JOYSTICK_DATA, (void *) (&joystick), sizeof(report));
}