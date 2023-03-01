#include <stdio.h>
#include <set>

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <pico/multicore.h>

#include <i2c_fifo.h>
#include <i2c_device.h>
#include <PwmIn.h>
#include "hub_utils.h"

std::set<uint8_t> modules;

static void update_addresses(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_RECEIVE) {
        //printf("Worker: Address receive\n");
        // only getting receives
        uint8_t received = i2c_read_byte(i2c);
        // if modules does not contain this address, add it
        auto success = modules.insert(received);
        if (success.second) {
            printf("Worker: Added module with ID: %#04X\n\n", received);
        }
    }
}

static void setup_worker() {
    gpio_init(hub::WORKER_SDA_PIN);
    gpio_set_function(hub::WORKER_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(hub::WORKER_SDA_PIN);

    gpio_init(hub::WORKER_SCL_PIN);
    gpio_set_function(hub::WORKER_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(hub::WORKER_SCL_PIN);

    uint baud = i2c_init(i2c0, I2C_BAUDRATE);
    printf("Actual baud is %d\n", baud);

    i2c_worker_init(i2c0, HUB_I2C_ADDRESS, &update_addresses);
}

static void core0_main() {
    gpio_init(hub::QUEEN_SDA_PIN);
    gpio_set_function(hub::QUEEN_SDA_PIN, GPIO_FUNC_I2C);
    // pull-ups are already active on worker side, this is just a fail-safe in case the wiring is faulty
    gpio_pull_up(hub::QUEEN_SDA_PIN);

    gpio_init(hub::QUEEN_SCL_PIN);
    gpio_set_function(hub::QUEEN_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(hub::QUEEN_SCL_PIN);

    i2c_init(i2c1, I2C_BAUDRATE);

    while(1) {

        for (uint8_t address : modules) {
            printf("Queen: Pinging device at %#04X\n", address);

            // Parse address for module type
            Module type = hub::parse_address(address);

            int count;
            uint8_t *buf;

            switch (type)
            {
            case BUTTON:
            case SWITCH:
                count = i2c_read_blocking(i2c1, address, buf, 1, false);
                break;
            
            case JOYSTICK:
                count = i2c_read_blocking(i2c1, address, buf, 3, false);
                break;

            case SLIDE_POTENTIOMETER:
            case ROTARY_POTENTIOMETER:
                count = i2c_read_blocking(i2c1, address, buf, 2, false);
                break;
            
            default:
                break;
            }

            if (count == PICO_ERROR_GENERIC) {
                printf("Device not recognized at address %#04X\n", address);
                modules.erase(address);
                continue;
            }
        }
        //int count = i2c_read_blocking(i2c1, I2C_WORKER_ADDRESS, (uint8_t *)&buf, 2, true);
        
    }
}

int main() {
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    // set pwm out pin
    // initialize pwm reader for pins

    setup_worker();

    // set up core 1 (usb)

    core0_main();
}