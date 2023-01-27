#include <i2c_fifo.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tusb.h>

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

static const uint I2C_MASTER_SDA_PIN = 6;
static const uint I2C_MASTER_SCL_PIN = 7;

static void run_master() {
    gpio_init(I2C_MASTER_SDA_PIN);
    gpio_set_function(I2C_MASTER_SDA_PIN, GPIO_FUNC_I2C);
    // pull-ups are already active on slave side, this is just a fail-safe in case the wiring is faulty
    gpio_pull_up(I2C_MASTER_SDA_PIN);

    gpio_init(I2C_MASTER_SCL_PIN);
    gpio_set_function(I2C_MASTER_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_MASTER_SCL_PIN);

    i2c_init(i2c1, I2C_BAUDRATE);

    while(true) {

        printf("Queen: Requesting data from worker\n");
        uint16_t buf;
        int count = i2c_read_blocking(i2c1, I2C_SLAVE_ADDRESS, (uint8_t *)&buf, 2, true);

        printf("Queen: Received value of %d from worker\n", buf);

        sleep_ms(1000);
    }
}

int main() {
    stdio_init_all();

    printf("waiting for usb host");
    while (!tud_cdc_connected()) {
      printf(".");
      sleep_ms(500);
    }
    printf("connected to usb\n\n");

    run_master();
}