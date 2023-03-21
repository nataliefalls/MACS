#include <mod_utils.h>
#include <hardware/gpio.h>

uint8_t read_digital_input(uint pin) {
    if(gpio_get_dir(pin) == GPIO_IN) {
        return gpio_get(pin);
    }
    return UINT8_MAX;
}

uint8_t module::get_address() {
    Module type;
    uint8_t id;

    #if BUTTON
    type = kButton;
    #endif

    #if SWITCH
    type = kSwitch;
    #endif

    #if SLIDER
    type = kSlider;
    #endif

    #if POTENTIOMETER
    type = kPotentiometer;
    #endif

    #if JOYSTICK
    type = kJoystick;
    #endif

    id = ID;
    invalid_params_if(MODULE, id > 31);

    uint8_t addr = type | (id << 3);
    invalid_params_if(I2C, addr >= 0x80); // 7-bit addresses
    invalid_params_if(I2C, (addr & 0x78) == 0 || (addr & 0x78) == 0x78);
    return addr;
}