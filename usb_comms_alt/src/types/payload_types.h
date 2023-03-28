#ifndef PAYLOAD_TYPES_H
#define PAYLOAD_TYPES_H

#include <stdint.h>
#include "coordinates.h"

// the state of the button on a module
typedef struct button_data_t {
    // - 0: not pressed
    // - 1: pressed
    uint8_t button;
} button_data_t;

// the state of the dpad on a module
typedef struct dpad_data_t {
    uint8_t dpad;
} dpad_data_t;

// the state of the joystick on a module
typedef struct joystick_data_t {
    // displacement of the joystick on the x-axis
    uint8_t x;
    // displacement of the joystick on the y-axis
    uint8_t y;
    // button on the joystick
    uint8_t button;
} joystick_data_t;

// a union of all the different types of data sent in a USB report
typedef union payload_t {
    module_coordinates_t coordinates;
    button_data_t button;
    dpad_data_t dpad;
    joystick_data_t joystick;
} payload_t;

#endif