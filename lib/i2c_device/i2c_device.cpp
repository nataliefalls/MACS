/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "i2c_device.h"
#include <hardware/irq.h>

typedef struct i2c_worker_t
{
    i2c_inst_t *i2c;
    i2c_worker_handler_t handler;
    bool transfer_in_progress;
} i2c_worker_t;

static i2c_worker_t i2c_workers[2];

static inline void finish_transfer(i2c_worker_t *worker) {
    if (worker->transfer_in_progress) {
        worker->handler(worker->i2c, I2C_WORKER_FINISH);
        worker->transfer_in_progress = false;
    }
}

static void __not_in_flash_func(i2c_worker_irq_handler)(i2c_worker_t *worker) {
    i2c_inst_t *i2c = worker->i2c;
    i2c_hw_t *hw = i2c_get_hw(i2c);

    uint32_t intr_stat = hw->intr_stat;
    if (intr_stat == 0) {
        return;
    }
    if (intr_stat & I2C_IC_INTR_STAT_R_TX_ABRT_BITS) {
        hw->clr_tx_abrt;
        finish_transfer(worker);
    }
    if (intr_stat & I2C_IC_INTR_STAT_R_START_DET_BITS) {
        hw->clr_start_det;
        finish_transfer(worker);
    }
    if (intr_stat & I2C_IC_INTR_STAT_R_STOP_DET_BITS) {
        hw->clr_stop_det;
        finish_transfer(worker);
    }
    if (intr_stat & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
        worker->transfer_in_progress = true;
        worker->handler(i2c, I2C_WORKER_RECEIVE);
    }
    if (intr_stat & I2C_IC_INTR_STAT_R_RD_REQ_BITS) {
        hw->clr_rd_req;
        worker->transfer_in_progress = true;
        worker->handler(i2c, I2C_WORKER_REQUEST);
    }
}

static void __not_in_flash_func(i2c0_worker_irq_handler)() {
    i2c_worker_irq_handler(&i2c_workers[0]);
}

static void __not_in_flash_func(i2c1_worker_irq_handler)() {
    i2c_worker_irq_handler(&i2c_workers[1]);
}

void i2c_worker_init(i2c_inst_t *i2c, uint8_t address, i2c_worker_handler_t handler) {
    assert(i2c == i2c0 || i2c == i2c1);
    assert(handler != NULL);

    uint i2c_index = i2c_hw_index(i2c);
    i2c_worker_t *worker = &i2c_workers[i2c_index];
    worker->i2c = i2c;
    worker->handler = handler;

    // Note: The I2C worker does clock stretching implicitly after a RD_REQ, while the Tx FIFO is empty.
    // There is also an option to enable clock stretching while the Rx FIFO is full, but we leave it
    // disabled since the Rx FIFO should never fill up (unless worker->handler() is way too slow).
    i2c_set_slave_mode(i2c, true, address);

    i2c_hw_t *hw = i2c_get_hw(i2c);
    // unmask necessary interrupts
    hw->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS | I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS | I2C_IC_INTR_MASK_M_STOP_DET_BITS | I2C_IC_INTR_MASK_M_START_DET_BITS;

    // enable interrupt for current core
    uint num = I2C0_IRQ + i2c_index;
    irq_set_exclusive_handler(num, i2c_index == 0 ? i2c0_worker_irq_handler : i2c1_worker_irq_handler);
    irq_set_enabled(num, true);
}

void i2c_worker_deinit(i2c_inst_t *i2c) {
    assert(i2c == i2c0 || i2c == i2c1);

    uint i2c_index = i2c_hw_index(i2c);
    i2c_worker_t *worker = &i2c_workers[i2c_index];
    assert(worker->i2c == i2c); // should be called after i2c_worker_init()

    worker->i2c = NULL;
    worker->handler = NULL;
    worker->transfer_in_progress = false;

    uint num = I2C0_IRQ + i2c_index;
    irq_set_enabled(num, false);
    irq_remove_handler(num, i2c_index == 0 ? i2c0_worker_irq_handler : i2c1_worker_irq_handler);

    i2c_hw_t *hw = i2c_get_hw(i2c);
    hw->intr_mask = I2C_IC_INTR_MASK_RESET;

    i2c_set_slave_mode(i2c, false, 0);
}
