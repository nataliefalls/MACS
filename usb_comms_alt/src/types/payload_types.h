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

inline bool sameButtonData(const button_data_t button1, const button_data_t button2) {
    return button1.button == button2.button;
}

typedef struct dpad_data_t {
    uint8_t dpad;
} dpad_data_t;

inline bool sameDpadData(const dpad_data_t dpad1, const dpad_data_t dpad2) {
    return dpad1.dpad == dpad2.dpad;
}

typedef struct joystick_data_t {
    uint8_t x;
    uint8_t y;
} joystick_data_t;
    
inline bool sameJoystickData(const joystick_data_t joystick1, const joystick_data_t joystick2) {
    return (joystick1.x == joystick2.x && joystick1.y == joystick2.y);
}

typedef union payload_t {
    module_coordinates_t coordinates;
    button_data_t button;
    dpad_data_t dpad;
    joystick_data_t joystick;
} payload_t;

#endif