#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "tusb.h"
#include "hardware/pwm.h"

#include "PwmIn.h"

#define NUM_OF_PINS 6

int main()
{
    // needed for printf
    stdio_init_all();
    printf("PwmIn on 4 pins\n");




    printf("waiting for usb host");
    while (!tud_cdc_connected()) {
      printf(".");
      sleep_ms(500);
    }
    printf("connected to usb\n\n");

    // set PwmIn
    uint pin_list[NUM_OF_PINS] = {0, 1, 2, 3, 14, 15};
    PwmIn my_PwmIn(pin_list, NUM_OF_PINS);
    
    const uint pwm_pin = 12;
    const uint pwm_pin_2 = 10;
    pwm_config cfg = pwm_get_default_config();
    pwm_init(pwm_gpio_to_slice_num(pwm_pin), &cfg, true);
    pwm_init(pwm_gpio_to_slice_num(pwm_pin_2), &cfg, true);

    gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
    gpio_set_function(pwm_pin_2, GPIO_FUNC_PWM);

    uint pwm1_slice = pwm_gpio_to_slice_num(pwm_pin);
    uint pwm2_slice = pwm_gpio_to_slice_num(pwm_pin_2);
    pwm_set_clkdiv_int_frac(pwm1_slice, 2, 0);
    pwm_set_clkdiv_int_frac(pwm2_slice, 2, 0);
    // pwm_set_wrap(pwm_gpio_to_slice_num(pwm_pin), 15);
    // pwm_set_chan_level()

    //const uint16_t pwm_levels[5] = {1923, 17394, 27, 5903, 58201};

    pwm_set_gpio_level(pwm_pin, 34008);
    pwm_set_gpio_level(pwm_pin_2, 2023);

    while (true)
    {
        // adviced empty (for now) function of sdk
        tight_loop_contents();

	for (int ii = 0; ii < NUM_OF_PINS; ii++) {
	  if (my_PwmIn.isConnected(ii)) {
	    uint16_t PW = my_PwmIn.read_PW(ii);
	    printf("PW_%d=%d ", ii , PW);
	  }
	  else
	    printf("PW_%d=NA ", ii);
	}

	printf("                  \r");
        sleep_ms(100);
    }
}

