#include <i2c_fifo.h>
#include <i2c_device.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hardware/adc.h>

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

// For this example, we run both the master and slave from the same board.
// You'll need to wire pin GP4 to GP6 (SDA), and pin GP5 to GP7 (SCL).
static const uint I2C_SLAVE_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN; // 4
static const uint I2C_SLAVE_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN; // 5

static const uint ADC_POTENTIOMETER_PIN = 26;   //pin 31
uint16_t potentiometer_value = 0;

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    printf("Worker: Received request for val from queen\n");
    uint8_t x[2];
    memcpy(x, &potentiometer_value, 2);
    i2c_write_byte(i2c, x[0]);
    i2c_write_byte(i2c, x[1]);
    printf("Worker: Sent value of %d to queen\n", potentiometer_value);
}

static void setup_slave() {
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);
    // configure I2C0 for slave mode
    i2c_slave_init(i2c0, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
}

int main() {
    
    stdio_init_all();
    
    adc_init();
    adc_gpio_init(ADC_POTENTIOMETER_PIN);
    adc_select_input(0);

    setup_slave();

    while(1) {
        potentiometer_value = adc_read();
        tight_loop_contents();
    }
}