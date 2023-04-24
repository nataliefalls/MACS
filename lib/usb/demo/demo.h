#pragma once
#include <stdint.h>
#include "IReportQueueController.h"

void init_adc_gpio();

void queueDemoReport(IReportQueueController *controller);

inline uint8_t adc_read_pin(uint32_t pin);

inline uint8_t read_button(uint32_t pin);