#ifndef REPORT_TYPES_H
#define REPORT_TYPES_H

#include <stdint.h>
#include "tusb.h"
#include "report_ids.h"

/**
 * MODULE CONNECTED
*/
typedef struct {
    uint8_t neighbor;
    uint8_t side;
} module_coordinates_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    module_coordinates_t coordinates;
} module_connected_report_t;

/**
 * MODULE DISCONNECTED
*/
typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
} module_disconnected_report_t;

/**
 * BUTTON
*/
typedef struct {
    uint8_t button;
} button_data_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    button_data_t button;
} button_report_t;

/**
 * DPAD
*/
typedef struct {
    uint8_t dpad;
} dpad_data_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    dpad_data_t dpad;
} dpad_report_t;

/**
 * JOYSTICK
*/
typedef struct {
    uint16_t x;
    uint16_t y;
} joystick_data_t;

typedef struct TU_ATTR_PACKED {
    uint8_t module_id;
    joystick_data_t joystick;
} joystick_report_t;

/**
 * PAYLOAD
*/
typedef union {
    module_coordinates_t coordinates;
    button_data_t button;
    dpad_data_t dpad;
    joystick_data_t joystick;
} payload_t;

typedef struct {
    report_id_t id;
    payload_t payload;
} report_t;

#endif