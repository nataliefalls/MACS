#include <stdio.h>
#include <set>

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <pico/multicore.h>

#include <i2c_modules.h>
#include <PwmIn.h>
#include "hub_utils.h"
#include <common.h>

int main() {
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    I2C_Hub hub(hub::QUEEN_SDA_PIN, hub::QUEEN_SCL_PIN, hub::WORKER_SDA_PIN, hub::WORKER_SCL_PIN);
    hub.setup();
    
    while(1) {
        hub.i2c_task();
    }

    // set pwm out pin
    // initialize pwm reader for pins

    // set up core 1 (usb)
}