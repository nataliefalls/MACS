#include <pico/stdlib.h>
#include <stdio.h>
#include "Wire.h"
#include <hardware/adc.h>

static const uint I2C_WORKER_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

// For this example, we run both the master and slave from the same board.
// You'll need to wire pin GP4 to GP6 (SDA), and pin GP5 to GP7 (SCL).
static const uint I2C_WORKER_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN; // 4
static const uint I2C_WORKER_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN; // 5

// Pin the Potentiometer should be connected to
static const uint ADC_POTENTIOMETER_PIN = 26;   //pin 31


// The slave implements a 256 byte memory. To write a series of bytes, the master first
// writes the memory address, followed by the data. The address is automatically incremented
// for each byte transferred, looping back to 0 upon reaching the end. Reading is done
// sequentially from the current memory address.

static void worker_on_request() {
    // read potentiometer data
    uint16_t value = adc_read();
    Wire.write(reinterpret_cast<uint8_t *>(value), 2);
    
}

static void setup_worker() {
    gpio_init(I2C_WORKER_SDA_PIN);
    gpio_set_function(I2C_WORKER_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_WORKER_SDA_PIN);

    gpio_init(I2C_WORKER_SCL_PIN);
    gpio_set_function(I2C_WORKER_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_WORKER_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);

    // setup Wire for slave mode
    //Wire.onReceive(slave_on_receive);
    Wire.onRequest(worker_on_request);
    // in this implementation, the user is responsible for initializing the I2C instance and GPIO
    // pins before calling Wire::begin()
    Wire.begin(I2C_WORKER_ADDRESS);
}

int main() {
    stdio_init_all();
    puts("\nI2C worker example with Wire API");

    adc_init();
    adc_gpio_init(ADC_POTENTIOMETER_PIN);
    adc_select_input(0);

    setup_worker();
}