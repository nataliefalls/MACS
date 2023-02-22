#ifndef REPORT_TYPE_DEFINITIONS_H
#define REPORT_TYPE_DEFINITIONS_H

#include <stdint.h>

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    uint8_t coordinates[3];
} module_connected_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
} module_disconnected_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    uint8_t button;
} button_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    uint8_t buttons;
} dpad_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    uint8_t x;
    uint8_t y;
} joystick_report_t;

typedef union {
    module_connected_report_t module_connected;
    module_disconnected_report_t module_disconnected;
    button_report_t button;
    dpad_report_t dpad;
    joystick_report_t joystick;
} payload_t;

#endif