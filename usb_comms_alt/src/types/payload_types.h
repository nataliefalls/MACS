#ifndef PAYLOAD_TYPES_H
#define PAYLOAD_TYPES_H

#include <stdint.h>

typedef struct {
    uint8_t neighbor;
    uint8_t side;
} module_coordinates_t;

typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
} button_state_t;

typedef struct {
    uint8_t button;
} button_data_t;

typedef struct {
    uint8_t dpad;
} dpad_data_t;

typedef struct {
    uint16_t x;
    uint16_t y;
} joystick_data_t;

typedef union {
    module_coordinates_t coordinates;
    button_data_t button;
    dpad_data_t dpad;
    joystick_data_t joystick;
} payload_t;

#endif