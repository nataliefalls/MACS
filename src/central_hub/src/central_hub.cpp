#include <stdio.h>
#include <set>

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <pico/multicore.h>

#include <i2c_modules.h>
#include <Pwm.h>
#include <i2c_device.h>
#include "hub_utils.h"
#include <common.h>

int main() {
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    uint pwm_in[6] = {
        PWM_IN_SIDE_1,
        PWM_IN_SIDE_2,
        PWM_IN_SIDE_3,
        PWM_IN_SIDE_4,
        PWM_IN_SIDE_5,
        PWM_IN_SIDE_6
    };
    uint pwm_out = PWM_OUT_PIN;

    Pwm module_pwm(pwm_out, pwm_in, 6);

    module_pwm.setPWMOut((uint16_t)HUB_I2C_ADDRESS);

    I2C_Hub hub(hub::QUEEN_SDA_PIN, hub::QUEEN_SCL_PIN, hub::WORKER_SDA_PIN, hub::WORKER_SCL_PIN);
    hub.setup();
    
    while(1) {
        hub.i2c_task();
    }

    // set pwm out pin
    // initialize pwm reader for pins

    // set up core 1 (usb)
}
