#include <i2c_fifo.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tusb.h>
#include <hardware/pwm.h>

#include <usb_descriptors.h>

static const uint I2C_WORKER_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

static const uint I2C_QUEEN_SDA_PIN = 6;
static const uint I2C_QUEEN_SCL_PIN = 7;

static const uint LED_PIN = 14;
static const uint MAX_PWM_LEVEL = 65535;
static const uint MAX_POT_LEVEL = 4095;

uint16_t map(uint16_t pot_level) {
    return pot_level * MAX_PWM_LEVEL / MAX_POT_LEVEL;
}

static void run_queen() {
    gpio_init(I2C_QUEEN_SDA_PIN);
    gpio_set_function(I2C_QUEEN_SDA_PIN, GPIO_FUNC_I2C);
    // pull-ups are already active on worker side, this is just a fail-safe in case the wiring is faulty
    gpio_pull_up(I2C_QUEEN_SDA_PIN);

    gpio_init(I2C_QUEEN_SCL_PIN);
    gpio_set_function(I2C_QUEEN_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_QUEEN_SCL_PIN);

    i2c_init(i2c1, I2C_BAUDRATE);

    while(true) {

        printf("Queen: Requesting data from worker\n");
        uint16_t buf;
        int count = i2c_read_blocking(i2c1, I2C_WORKER_ADDRESS, (uint8_t *)&buf, 2, true);

        printf("Queen: Received value of %d from worker\n", buf);
        pwm_set_gpio_level(LED_PIN, map(buf));

        sleep_ms(10);
    }
}

int main() {
    stdio_init_all();

    uint sliceNum = pwm_gpio_to_slice_num(LED_PIN);
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_init(sliceNum, &config, true);

    // Needed for USB debugging output
    // printf("waiting for usb host");
    // while (!tud_cdc_connected()) {
    //   printf(".");
    //   sleep_ms(500);
    // }
    // printf("connected to usb\n\n");

    run_queen();
}