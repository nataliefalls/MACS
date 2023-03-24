#pragma once
#include <stdint.h>

void init_adc_gpio();

void send_demo_report();

inline uint16_t adc_read_pin(uint pin);

inline uint8_t read_button(uint pin);