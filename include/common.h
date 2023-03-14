#ifndef MACS_COMMON
#define MACS_COMMON

// HUB DOES NOT USE SIDE 1
#define PWM_OUT_PIN 13
#define PWM_IN_SIDE_1 22
#define PWM_IN_SIDE_2 21
#define PWM_IN_SIDE_3 17
#define PWM_IN_SIDE_4 15
#define PWM_IN_SIDE_5 14
#define PWM_IN_SIDE_6 13

enum Module : uint8_t {
    BUTTON = 0,
    SWITCH,
    SLIDE_POTENTIOMETER,
    ROTARY_POTENTIOMETER,
    JOYSTICK
};

#endif