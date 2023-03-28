/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _I2C_WORKER_H_
#define _I2C_WORKER_H_

#include <functional>
#include <hardware/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \file i2c_worker.h
 *
 * \brief I2C worker setup.
 */

#define I2C_BAUDRATE 100000
#define HUB_I2C_ADDRESS 0x77
#define WORKER_I2C_SDA_PIN PICO_DEFAULT_I2C_SDA_PIN
#define WORKER_I2C_SCL_PIN PICO_DEFAULT_I2C_SCL_PIN

/**
 * \brief I2C worker event types.
 */
typedef enum i2c_worker_event_t
{
    I2C_WORKER_RECEIVE, /**< Data from master is available for reading. worker must read from Rx FIFO. */
    I2C_WORKER_REQUEST, /**< Master is requesting data. worker must write into Tx FIFO. */
    I2C_WORKER_FINISH, /**< Master has sent a Stop or Restart signal. worker may prepare for the next transfer. */
} i2c_worker_event_t;

/**
 * \brief I2C worker event handler
 * 
 * The event handler will run from the I2C ISR, so it should return quickly (under 25 us at 400 kb/s).
 * Avoid blocking inside the handler and split large data transfers across multiple calls for best results.
 * When sending data to master, up to `i2c_get_write_available()` bytes can be written without blocking.
 * When receiving data from master, up to `i2c_get_read_available()` bytes can be read without blocking.
 * 
 * \param i2c worker I2C instance.
 * \param event Event type.
 */
typedef void (*i2c_worker_handler_t)(i2c_inst_t *i2c, i2c_worker_event_t event);

/**
 * \brief Configure I2C instance for worker mode.
 * 
 * \param i2c I2C instance.
 * \param address 7-bit worker address.
 * \param handler Called on events from I2C master. It will run from the I2C ISR, on the CPU core
 *                where the worker was initialized.
 */
void i2c_worker_init(i2c_inst_t *i2c, uint8_t address, std::function<void(i2c_inst_t*, i2c_worker_event_t)> handler);

/**
 * \brief Restore I2C instance to master mode.
 *
 * \param i2c I2C instance.
 */
void i2c_worker_deinit(i2c_inst_t *i2c);

#ifdef __cplusplus
}
#endif

#endif // _I2C_WORKER_H_
