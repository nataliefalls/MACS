#include <pico/stdlib.h>
#include <hardware/pwm.h>

#include <i2c_modules.h>
#include <Pwm.h>
#include <mod_utils.h>


#define NUM_PWM_PINS 6

int main() {
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    uint8_t addr = module::get_address();

    
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

    module_pwm.setPWMOut((uint32_t)addr);
    // sleep for some time to let neigbors initialize if necessary
    sleep_us(500000);

    bool found_neighbor = false;
    uint side = 0;
    while(!found_neighbor) {
      side = (side + 1) % NUM_PWM_PINS;
      if (module_pwm.isConnected(side))
	break;
    }

    uint8_t neighbor_address = module_pwm.read_PW(side) & 0x00FF;

    I2C_Module module(addr, side, neighbor_address,
		      PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
		      parse_address(addr));
    
    module.setup();

    while(1) {
        tight_loop_contents();
    }
}
