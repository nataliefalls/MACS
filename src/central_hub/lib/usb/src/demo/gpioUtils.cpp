#include "gpioUtils.h"
#include "hardware/adc.h"

void init_adc_gpio() {
  gpio_init(BUTTON_PIN);
  gpio_init(JOYSTICK_BUTTON_PIN);
  gpio_init(MODULE_CONNECT_PIN);

  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
  gpio_set_dir(MODULE_CONNECT_PIN, GPIO_IN);

  gpio_pull_down(BUTTON_PIN);
  gpio_pull_down(JOYSTICK_BUTTON_PIN);
  gpio_pull_down(MODULE_CONNECT_PIN);

  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
}

uint8_t adc_read_pin(uint32_t pin) {
  adc_select_input(pin);
  return adc_read() >> 4;
}

uint16_t adc_read_pin16(uint32_t pin) {
  adc_select_input(pin);
  return adc_read();
}

joystick_data_t read_joystick() {
  joystick_data_t joystick = {
    adc_read_pin(JOYSTICK_X_PIN),
    adc_read_pin(JOYSTICK_Y_PIN),
    read_button(JOYSTICK_BUTTON_PIN),
  };
  return joystick;
}

uint8_t read_button(uint32_t pin) {
  return gpio_get(pin) ? 1 : 0;
}