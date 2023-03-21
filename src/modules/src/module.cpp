#include <pico/stdlib.h>
#include <hardware/pwm.h>

#include <i2c_modules.h>
#include <Pwm.h>


int main() {
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    I2C_Module module(0x01, PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, parse_address(0x01));

    module.setup();

    while(1) {
        tight_loop_contents();
    }
}
