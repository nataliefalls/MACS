#include <i2c_fifo.h>
#include <i2c_device.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hardware/adc.h>
//#include <random>
#include <time.h>
#include <tusb.h>

static const uint8_t MODULE_I2C_ADDRESS = 0x17;
static const uint8_t HUB_I2C_ADDRESS = 0x01;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

static const uint I2C_WORKER_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN; // 4
static const uint I2C_WORKER_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN; // 5

uint8_t rand_val;

static void i2c_worker_handler(i2c_inst_t *i2c, i2c_worker_event_t event) {
    printf("Worker: Received request for val from queen\n");
    i2c_write_byte(i2c, rand_val);
    printf("Worker: Sent value of %d to queen\n", rand_val);
}

static void send_address() {
    gpio_init(I2C_WORKER_SDA_PIN);
    gpio_set_function(I2C_WORKER_SDA_PIN, GPIO_FUNC_I2C);
    // pull-ups are already active on worker side, this is just a fail-safe in case the wiring is faulty
    gpio_pull_up(I2C_WORKER_SDA_PIN);

    gpio_init(I2C_WORKER_SCL_PIN);
    gpio_set_function(I2C_WORKER_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_WORKER_SCL_PIN);

    i2c_init(i2c1, I2C_BAUDRATE);

    printf("Sending this module's address %#04X to the central hub at %#04X\n", MODULE_I2C_ADDRESS, HUB_I2C_ADDRESS);

    
    int success = i2c_write_timeout_us(i2c1, HUB_I2C_ADDRESS, &MODULE_I2C_ADDRESS, sizeof(MODULE_I2C_ADDRESS), true, 1000);
    while(success == PICO_ERROR_GENERIC || success == PICO_ERROR_TIMEOUT) {
        printf("Could not send to central hub... trying again\n");
        success = i2c_write_timeout_us(i2c1, HUB_I2C_ADDRESS, &MODULE_I2C_ADDRESS, sizeof(MODULE_I2C_ADDRESS), true, 1000);
    }
    //int success = i2c_write_blocking(i2c1, HUB_I2C_ADDRESS, &MODULE_I2C_ADDRESS, 1, false);

    i2c_deinit(i2c1);

    printf("Deinitializing this I2C core\n");
}

static void setup_worker() {

    i2c_init(i2c0, I2C_BAUDRATE);
    // configure I2C0 for worker mode
    i2c_worker_init(i2c0, MODULE_I2C_ADDRESS, &i2c_worker_handler);
    printf("Initialized Worker\n");
}

int main() {
    
    stdio_init_all();

    printf("waiting for usb host");
    while (!tud_cdc_connected()) {
      printf(".");
      sleep_ms(500);
    }
    printf("connected to usb\n\n");

    rand_val = 0x4b;
    printf("Random value is %d\n", rand_val);

    sleep_ms(5000);

    send_address();
    setup_worker();

    while(1) {
        tight_loop_contents();
    }
}