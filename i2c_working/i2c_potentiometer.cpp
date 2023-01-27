/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <i2c_fifo.h>
#include <i2c_worker.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hardware/adc.h>
#include <tusb.h>

#include <pio_usb.h>

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

// For this example, we run both the master and slave from the same board.
// You'll need to wire pin GP4 to GP6 (SDA), and pin GP5 to GP7 (SCL).
static const uint I2C_SLAVE_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN; // 4
static const uint I2C_SLAVE_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN; // 5
static const uint I2C_MASTER_SDA_PIN = 6;
static const uint I2C_MASTER_SCL_PIN = 7;

// Pin the Potentiometer should be connected to
static const uint ADC_POTENTIOMETER_PIN = 26;   //pin 31
static const uint LED_PIN = 9;

uint16_t potentiometer_value = 0;

// The slave implements a 256 byte memory. To write a series of bytes, the master first
// writes the memory address, followed by the data. The address is automatically incremented
// for each byte transferred, looping back to 0 upon reaching the end. Reading is done
// sequentially from the current memory address.
// static struct
// {
//     uint8_t mem[256];
//     uint8_t mem_address;
//     bool mem_address_written;
// } context;

// Our handler is called from the I2C ISR, so it must complete quickly. Blocking calls /
// printing to stdio may interfere with interrupt handling.
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    printf("Worker: Received request for val from queen\n");
    uint8_t x[2];
    memcpy(x, &potentiometer_value, 2);
    i2c_write_byte(i2c, x[0]);
    i2c_write_byte(i2c, x[1]);
    printf("Worker: Sent value of %d to queen\n", potentiometer_value);

    // switch (event) {
    // case I2C_SLAVE_RECEIVE: // master has written some data
    //     if (!context.mem_address_written) {
    //         // writes always start with the memory address
    //         context.mem_address = i2c_read_byte(i2c);
    //         context.mem_address_written = true;
    //     } else {
    //         // save into memory
    //         context.mem[context.mem_address] = i2c_read_byte(i2c);
    //         context.mem_address++;
    //     }
    //     break;
    // case I2C_SLAVE_REQUEST: // master is requesting data
    //     // load from memory
    //     i2c_write_byte(i2c, context.mem[context.mem_address]);
    //     context.mem_address++;
    //     break;
    // case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
    //     context.mem_address_written = false;
    //     break;
    // default:
    //     break;
    // }
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
        printf("Reading potentiometer value\n");
        potentiometer_value = adc_read();
        printf("Read potentiometer value of %d\n", potentiometer_value);

        printf("Queen: Requesting data from worker\n");
        uint16_t buf;
        int count = i2c_read_blocking(i2c1, I2C_SLAVE_ADDRESS, (uint8_t *)&buf, 2, true);

        printf("Queen: Received value of %d from worker", buf);

        sleep_ms(1000);
    }

    // for (uint8_t mem_address = 0;; mem_address = (mem_address + 32) % 256) {
    //     char msg[32];
    //     snprintf(msg, sizeof(msg), "Hello, I2C slave! - 0x%02X", mem_address);
    //     uint8_t msg_len = strlen(msg);

    //     uint8_t buf[32];
    //     buf[0] = mem_address;
    //     memcpy(buf + 1, msg, msg_len);
    //     // write message at mem_address
    //     printf("Write at 0x%02X: '%s'\n", mem_address, msg);
    //     int count = i2c_write_blocking(i2c1, I2C_SLAVE_ADDRESS, buf, 1 + msg_len, false);
    //     if (count < 0) {
    //         puts("Couldn't write to slave, please check your wiring!");
    //         return;
    //     }
    //     hard_assert(count == 1 + msg_len);

    //     // seek to mem_address
    //     count = i2c_write_blocking(i2c1, I2C_SLAVE_ADDRESS, buf, 1, true);
    //     hard_assert(count == 1);
    //     // partial read
    //     uint8_t split = 5;
    //     count = i2c_read_blocking(i2c1, I2C_SLAVE_ADDRESS, buf, split, true);
    //     hard_assert(count == split);
    //     buf[count] = '\0';
    //     printf("Read  at 0x%02X: '%s'\n", mem_address, buf);
    //     hard_assert(memcmp(buf, msg, split) == 0);
    //     // read the remaining bytes, continuing from last address
    //     count = i2c_read_blocking(i2c1, I2C_SLAVE_ADDRESS, buf, msg_len - split, false);
    //     hard_assert(count == msg_len - split);
    //     buf[count] = '\0';
    //     printf("Read  at 0x%02X: '%s'\n", mem_address + split, buf);
    //     hard_assert(memcmp(buf, msg + split, msg_len - split) == 0);

    //     puts("");
    //     sleep_ms(2000);
    // }
}

int main() {
    stdio_init_all();
    puts("\nI2C slave example");

    printf("waiting for usb host");
    while (!tud_cdc_connected()) {
      printf(".");
      sleep_ms(500);
    }
    printf("connected to usb\n\n");

    adc_init();
    adc_gpio_init(ADC_POTENTIOMETER_PIN);
    adc_select_input(0);

    setup_slave();
    run_master();
}
