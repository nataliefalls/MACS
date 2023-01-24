#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "tusb.h"
#include "Wire.h"
#include <hardware/pwm.h>
#include <cstring>

static const uint I2C_WORKER_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

// For this example, we run both the queen and worker from the same board.
// You'll need to wire pin GP4 to GP6 (SDA), and pin GP5 to GP7 (SCL).
static const uint I2C_QUEEN_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN;
static const uint I2C_QUEEN_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN;

// Potentiometer value from worker
static const uint LED_PIN = 14;
static const uint MAX_PWM_LEVEL = 65535;

uint16_t map_brightness(uint16_t res) {
    double fraction = static_cast<double>(res)/0xFFF;
    uint16_t pwm_level = static_cast<uint16_t>(fraction * MAX_PWM_LEVEL);
    puts("Setting LED to PWM level " + pwm_level);
    return pwm_level;
}

static void run_queen() {
    gpio_init(LED_PIN);
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);

    uint sliceNum = pwm_gpio_to_slice_num(LED_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_init(sliceNum, &config, true);

    pwm_set_gpio_level(LED_PIN, 0);

    gpio_init(I2C_QUEEN_SDA_PIN);
    gpio_set_function(I2C_QUEEN_SDA_PIN, GPIO_FUNC_I2C);
    // pull-ups are already active on worker side, this is just a fail-safe in case the wiring is faulty
    gpio_pull_up(I2C_QUEEN_SDA_PIN);

    gpio_init(I2C_QUEEN_SCL_PIN);
    gpio_set_function(I2C_QUEEN_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_QUEEN_SCL_PIN);

    i2c_init(i2c1, I2C_BAUDRATE);

    // setup Wire for queen mode on i2c1
    Wire1.begin();

    while (true) {
        uint16_t resistance = 0;
        uint8_t buf[2];

        uint8_t result = Wire1.requestFrom(I2C_WORKER_ADDRESS, 2, false);
        if (result == 0) {
            printf("i2c request error");
        }
        hard_assert(result == sizeof(buf)/sizeof(uint8_t));
        for (size_t i = 0; i < result; i++) {
            buf[i] = Wire1.read();
        }

        std::memcpy(&resistance, &buf, sizeof(resistance));
        puts("Value received from worker is " + resistance);

        pwm_set_gpio_level(LED_PIN, map_brightness(resistance));

        sleep_ms(1);
    }
}

/*******************************************************************************
 * Main
 */
int main() {
    stdio_init_all();
    printf("waiting for usb host");
    while (!tud_cdc_connected()) {
      printf(".");
      sleep_ms(500);
    }
    printf("\nusb host detected!\n");

    run_queen();
}