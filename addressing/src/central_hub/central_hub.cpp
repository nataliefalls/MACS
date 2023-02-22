#include <stdio.h>
#include <string.h>
#include <set>
#include <memory>

#include <pico/stdlib.h>
#include <tusb.h>
#include <hardware/pwm.h>

#include <i2c_fifo.h>
#include <i2c_device.h>

static const uint I2C_BAUDRATE = 100000; // 100 kHz
static const uint8_t I2C_WORKER_ADDRESS = 0x01;

std::shared_ptr<std::set<uint8_t>> modules;

static const uint I2C_QUEEN_SDA_PIN = 6;
static const uint I2C_QUEEN_SCL_PIN = 7;
static const uint I2C_WORKER_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN;
static const uint I2C_WORKER_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN;

static void update_addresses(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_RECEIVE) {
        //printf("Worker: Address receive\n");
        // only getting receives
        uint8_t received = i2c_read_byte(i2c);
        // if modules does not contain this address, add it
        auto success = modules->insert(received);
        if (success.second) {
            //printf("Worker: Added module with ID: %#04X\n", received);
        }
    }
}

static void setup_worker() {
    gpio_init(I2C_WORKER_SDA_PIN);
    gpio_set_function(I2C_WORKER_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_WORKER_SDA_PIN);

    gpio_init(I2C_WORKER_SCL_PIN);
    gpio_set_function(I2C_WORKER_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_WORKER_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);

    i2c_worker_init(i2c0, I2C_WORKER_ADDRESS, &update_addresses);
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

        for(auto x : *modules) {
            //printf("Queen: Pinging device at %#04X\n", x);
            uint8_t buf;
            int count = i2c_read_blocking(i2c1, x, &buf, 1, false);
            if (count == PICO_ERROR_GENERIC) {
                //printf("Device not recognized at address %#04X\n", x);
                continue;
            }

            //printf("Queen: Received value of %d from worker %#04X\n", buf, x);
        }
        //int count = i2c_read_blocking(i2c1, I2C_WORKER_ADDRESS, (uint8_t *)&buf, 2, true);
        sleep_ms(10);
    }
}

int main() {
    stdio_init_all();

    // Needed for USB debugging output
    // printf("waiting for usb host");
    // while (!tud_cdc_connected()) {
    //   printf(".");
    //   sleep_ms(500);
    // }
    // printf("connected to usb\n\n");

    std::set<uint8_t> module_set;
    modules = std::make_shared<std::set<uint8_t>>(module_set);

    setup_worker();
    run_queen();
}