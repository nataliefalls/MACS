#include <mod_utils.h>
//#include <hardware/gpio.h>

uint8_t module::map_16_to_8(uint16_t x) {
    return static_cast<uint8_t>(x >> 4);
}

#if DIGITAL
uint module::get_digital_pin() {
    #if BUTTON || SWITCH
    return 10;
    #elif JOYSTICK
    return 18;
    #endif
}
#endif

uint8_t module::get_address() {
    ModuleType type;
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
    // printf("\nDigital Pin is GP%d\n", digital_pin);

    gpio_init(digital_pin);
    gpio_set_dir(digital_pin, GPIO_IN);
    #if BUTTON || SWITCH
    gpio_pull_down(digital_pin);
    #endif
    #if JOYSTICK
    gpio_pull_up(digital_pin);
    #endif
    #endif
    #if ANALOG
    adc_init();
    uint analog_pin = 26;
    adc_gpio_init(0);
    #if JOYSTICK
    analog_pin = 27;
    adc_gpio_init(1);
    #endif
    adc_select_input(0);
    #endif

}

uint8_t module::get_input(uint8_t* buf) {
    uint8_t i = 0;

    #if DIGITAL
    // Switch or Button input (including joystick button)
    uint digital_pin = get_digital_pin();
    uint8_t digital_in = gpio_get(digital_pin);
    #if JOYSTICK
    digital_in != digital_in;
    #endif
    buf[i++] = digital_in;
    // printf("\nBuffer: %d", buf[0]);
    #endif

    #if ANALOG
    // Potentiometer or X-axis of joystick
    adc_select_input(0);
    uint8_t analog_in;
    auto in = adc_read();
    analog_in = map_16_to_8(in);
    buf[i++] = analog_in;

    #if JOYSTICK
    // Y-axis of joystick
    adc_select_input(1);
    in = adc_read();
    analog_in = map_16_to_8(in);
    buf[i++] = analog_in;
    #endif

    #endif
    return i;
}