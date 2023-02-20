#ifndef REPORT_TYPE_DEFINITIONS_H
#define REPORT_TYPE_DEFINITIONS_H

#include <stdint.h>

typedef struct TU_ATTR_PACKED {
    uint8_t report_id;
    uint8_t module_id;
    uint8_t coordinates[3];
} module_coordinates_t;

typedef struct TU_ATTR_PACKED {
    uint8_t report_id;
    uint8_t module_id;
} module_disconnected_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t report_id;
    uint8_t module_id;
    uint8_t button;
} button_data_t;

typedef struct TU_ATTR_PACKED {
    uint8_t report_id;
    uint8_t module_id;
    uint8_t buttons;
} dpad_data_t;

typedef struct TU_ATTR_PACKED {
    uint8_t report_id;
    uint8_t module_id;
    uint8_t x;
    uint8_t y;
} joystick_data_t;

#endif