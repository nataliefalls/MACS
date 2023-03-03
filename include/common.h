#ifndef MACS_COMMON
#define MACS_COMMON

// placeholder values for now
#define PWM_OUT_PIN 1
#define PWM_IN_SIDE_1 2
#define PWM_IN_SIDE_2 3
#define PWM_IN_SIDE_3 4
#define PWM_IN_SIDE_4 5
#define PWM_IN_SIDE_5 6
#define PWM_IN_SIDE_6 7

enum Module : uint8_t {
    BUTTON,
    SWITCH,
    SLIDE_POTENTIOMETER,
    ROTARY_POTENTIOMETER,
    JOYSTICK
};

#endif