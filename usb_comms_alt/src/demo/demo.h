#pragma once
#include <stdint.h>

void init_adc_gpio();

inline uint16_t adc_read_pin(uint pin);

void send_demo_report();