#include "report_queue.h"
#include "usb_controller.h"

bool register_module(module_connected_report_t report) {
    report_t newReport = {
        .id = report.module_id,
        .payload.coordinates = report.coordinates
    };
    queue_push(newReport);
}

bool unregister_module(module_disconnected_report_t report) {
    report_t newReport = {
        .id = report.module_id
    };
    queue_push(newReport);
}

bool send_button_report(button_report_t report) {
    report_t newReport = {
        .id = report.module_id,
        .payload.button = report.button
    };
    queue_push(newReport);
}

bool send_dpad_report(dpad_report_t report) {
    report_t newReport = {
        .id = report.module_id,
        .payload.dpad = report.dpad
    };
    queue_push(newReport);
}

bool send_joystick_report(joystick_report_t report) {
    report_t newReport = {
        .id = report.module_id,
        .payload.joystick = report.joystick
    };
    queue_push(newReport);
}