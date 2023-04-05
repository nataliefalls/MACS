// #include <memory>

#include <pico/stdlib.h>
#include <hardware/pwm.h>

#include <i2c_modules.h>
#include <Pwm.h>
#include <mod_utils.h>
// #include "tusb.h"
// #include <cstdio>


#define NUM_PWM_PINS 6

int main() {
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    // printf("waiting for usb host");
    // while (!tud_cdc_connected()) {
    //   printf(".");
    //   sleep_ms(500);
    // }
    // printf("connected to usb\n\n");

    uint8_t addr = module::get_address();
    ModuleType type = parse_address(addr);

    module::init_inputs();
    
    uint pwm_in[6] = {
		   PWM_IN_SIDE_1,
		   PWM_IN_SIDE_2,
		   PWM_IN_SIDE_3,
		   PWM_IN_SIDE_4,
		   PWM_IN_SIDE_5,
		   PWM_IN_SIDE_6
    };
    uint pwm_out = PWM_OUT_PIN;

    Pwm module_pwm(pwm_out, pwm_in, NUM_PWM_PINS);

    module_pwm.setPWMOut((uint16_t)addr);
    // sleep for some time to let neigbors initialize if necessary
    sleep_us(500000);

    uint8_t neighbor_address[6];
    uint side = 0;
    for (int ii = 0; ii < 6; ii++) {
        if (module_pwm.isConnected(ii))
            neighbor_address[ii] = module_pwm.read_PW(side) & 0x00FF;
        else
            neighbor_address[ii] = 0x0000;
    }

    I2C_Module module(addr, neighbor_address,
                      PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                      type);
    
    module.setup();

    uint8_t* buf = new uint8_t[hw_size_from_type(type)];
    uint8_t count;

    while(1) {
        count = module::get_input(buf);
        assert(count == hw_size_from_type(type));
        module.update_input(buf);
        tight_loop_contents();
    }

    delete buf;
}
