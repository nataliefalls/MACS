#pragma once

#include <stdint.h>
#include "payload_types.h"
#include "pico.h"

#define buttonModuleID 0x68
#define joystickModuleID 0x69

#define MODULE_CONNECT_PIN 13
#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1
#define JOYSTICK_BUTTON_PIN 15

void init_adc_gpio();

uint8_t read_button(uint32_t pin);

joystick_data_t read_joystick();

uint16_t adc_read_pin16(uint32_t pin);

uint8_t adc_read_pin(uint32_t pin);