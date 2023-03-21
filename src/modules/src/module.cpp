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

    module.setup();

    while(1) {
        tight_loop_contents();
    }
}
