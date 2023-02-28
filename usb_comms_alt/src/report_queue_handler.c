#include "report_queue_handler.h"
#include "report_queue.h"
#include "report_types.h"
#include "tusb.h"

bool send_report(report_t report) {
    switch (report.id) {
        case REPORT_ID_MODULE_CONNECTED: {
            module_connected_report_t connected = {
                .module_id = report.id,
                .coordinates = report.payload.coordinates
            };
            return tud_hid_report(REPORT_ID_MODULE_CONNECTED, (void *) (&connected), sizeof(connected));
        }
        case REPORT_ID_MODULE_DISCONNECTED: {
            module_disconnected_report_t disconnected = {
                .module_id = report.id
            };
            return tud_hid_report(REPORT_ID_MODULE_DISCONNECTED, (void *) (&disconnected), sizeof(report));
        }
        case REPORT_ID_BUTTON_DATA: {
            button_report_t button = {
                .module_id = report.id,
                .button = report.payload.button
            };
            return tud_hid_report(REPORT_ID_BUTTON_DATA, (void *) (&button), sizeof(report));
        }
        case REPORT_ID_DPAD_DATA: {
            dpad_report_t dpad = {
                .module_id = report.id,
                .dpad = report.payload.dpad
            };
            return tud_hid_report(REPORT_ID_DPAD_DATA, (void *) (&dpad), sizeof(report));
        }
        case REPORT_ID_JOYSTICK_DATA: {
            joystick_report_t joystick = {
                .module_id = report.id,
                .joystick = report.payload.joystick
            };
            return tud_hid_report(REPORT_ID_JOYSTICK_DATA, (void *) (&joystick), sizeof(report));
        }
        default: return false;
    }
}