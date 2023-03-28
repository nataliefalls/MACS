#include <mod_utils.h>
//#include <hardware/gpio.h>

uint8_t read_digital_input(uint pin) {
    if(gpio_get_dir(pin) == GPIO_IN) {
        return gpio_get(pin);
    }
    return UINT8_MAX;
}

uint module::get_digital_pin() {
    #if BUTTON || SWITCH
    return 10;
    #elif JOYSTICK
    return 18;
    #endif
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

void module::init_inputs() {
    #if DIGITAL
    uint digital_pin = get_digital_pin();

    gpio_init(digital_pin);
    gpio_set_dir(digital_pin, GPIO_IN);
    #endif
    #if ANALOG
    uint analog_pin = 26;
    adc_gpio_init(analog_pin);
    #if JOYSTICK
    analog_pin = 28;
    adc_gpio_init(analog_pin);
    #endif
    #endif

}

uint8_t module::get_input(uint pin, uint8_t* buf) {
    #if DIGITAL
    uint digital_pin = get_digital_pin();
    uint8_t digital_in = gpio_get(digital_pin);
    #endif
    #if ANALOG
    adc_select_input(0);
    uint16_t analog_in;
    #endif
}