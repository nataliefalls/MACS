#include "report_queue.h"
#include "usb_controller.h"

bool module_connected_report(uint8_t moduleID, module_coordinates_t coordinates) {
    report_t newReport = {
        .reportID = REPORT_ID_MODULE_CONNECTED,
        .moduleID = moduleID,
        .payload.coordinates = coordinates
    };
    queue_push(newReport);
}

bool module_disconnected_report(uint8_t moduleID) {
    report_t newReport = {
        .reportID = REPORT_ID_MODULE_DISCONNECTED,
        .moduleID = moduleID,
    };
    queue_push(newReport);
}

bool button_report(uint8_t moduleID, button_data_t button) {
    report_t newReport = {
        .reportID = REPORT_ID_BUTTON_DATA,
        .moduleID = moduleID,
        .payload.button = button
    };
    queue_push(newReport);
}

bool dpad_report(uint8_t moduleID, dpad_data_t dpad) {
    report_t newReport = {
        .reportID = REPORT_ID_DPAD_DATA,
        .moduleID = moduleID,
        .payload.dpad = dpad
    };
    queue_push(newReport);
}

bool joystick_report(uint8_t moduleID, joystick_data_t joystick) {
    report_t newReport = {
        .reportID = REPORT_ID_JOYSTICK_DATA,
        .moduleID = moduleID,
        .payload.joystick = joystick
    };
    queue_push(newReport);
}

uint8_t queue_space_remaining() {
    return space_remaining();
}