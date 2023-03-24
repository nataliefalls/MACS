#ifndef PAYLOAD_TYPES_H
#define PAYLOAD_TYPES_H

#include <stdint.h>

typedef struct module_coordinates_t {
    uint8_t neighbor;
    uint8_t side;
} module_coordinates_t;

typedef struct button_data_t {
    uint8_t button;
} button_data_t;

typedef struct dpad_data_t {
    uint8_t dpad;
} dpad_data_t;

typedef struct joystick_data_t {
    uint8_t x;
    uint8_t y;
} joystick_data_t;

typedef union payload_t {
    module_coordinates_t coordinates;
    button_data_t button;
    dpad_data_t dpad;
    joystick_data_t joystick;
} payload_t;

#endif