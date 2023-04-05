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
    
enum ModuleType : uint8_t {
    kButton,
    kSwitch,
    kSlider,
    kPotentiometer,
    kJoystick
};

inline ModuleType parse_address(uint8_t address) {
    return static_cast<ModuleType>(address & 0x07);
}

#endif