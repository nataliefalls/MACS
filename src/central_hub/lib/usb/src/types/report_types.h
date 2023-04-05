#ifndef REPORT_TYPES_H
#define REPORT_TYPES_H

#include "tusb.h"
#include "report_ids.h"
#include "payload_types.h"

typedef struct TU_ATTR_PACKED {
    uint8_t moduleID;
    module_coordinates_t coordinates;
} module_connected_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t moduleID;
} module_disconnected_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t moduleID;
    digital_data_t digital;
} digital_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t moduleID;
    analog_data_t analog;
} analog_report_t;

typedef struct TU_ATTR_PACKED {
    uint8_t moduleID;
    joystick_data_t joystick;
} joystick_report_t;

typedef struct report_t {
    report_id_t reportID;
    uint8_t moduleID;
    payload_t payload;
} report_t;

#endif