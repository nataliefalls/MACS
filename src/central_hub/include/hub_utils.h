#include <pico/stdlib.h>
#include <common.h>

namespace hub {
static const uint QUEEN_SDA_PIN = 6;
static const uint QUEEN_SCL_PIN = 7;
static const uint WORKER_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN;
static const uint WORKER_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN;

static Module parse_address(uint8_t address) {
    // sample
    return Module::BUTTON;
}
}