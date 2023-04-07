#include <cstdio>

#include "i2c_module.h"

using namespace std::placeholders;

/* I2C Module Definition */

I2C_Module::I2C_Module(uint8_t address, uint8_t neighbor_address[],
                       uint sda, uint scl, ModuleType type,
                       std::function<void(i2c_inst_t *, i2c_worker_event_t)> handler)
    : I2C_Base(address, sda, scl),
      _neighbor_address(neighbor_address), _handler(handler) {
    hw_type = type;
    hw_size = hw_size_from_type(hw_type);
    hw_status = new uint8_t[hw_size];
    request_type_register = 0;
}

I2C_Module::~I2C_Module() {
    delete hw_status;
}

// void I2C_Module::worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event) {
//     if (event == I2C_WORKER_REQUEST) {
//         //printf("Worker: Received request for val from queen\n");
//         i2c_write_raw_blocking(i2c, hw_status, hw_size);
//     }
// }

void I2C_Module::update_input(uint8_t* hardware_input) {
    // Data is copied to disallow input functions from mutating a private I2C_Module data member
    std::copy(hardware_input, hardware_input + hw_size, hw_status);
    assert(hardware_input != hw_status);
    assert(hardware_input[0] == hw_status[0]);
    // for (int i = 0; i < hw_size; i++) {
    //     printf("%d ", hw_status[i]);
    // }
}

const uint8_t* I2C_Module::get_status() const {
    return hw_status;
}

uint8_t I2C_Module::get_size() {
    return hw_size;
}

void I2C_Module::setup() {
    initialize_i2c(i2c0, sda, scl);

    // create buffer containing address and neighbor data to send to hub
    init_module_buffer send_buffer;
    send_buffer.addr = addr;
    for (int ii = 0; ii < 6; ii++) {
      send_buffer.neighbor_address[ii] = _neighbor_address[ii];
    }
    printf("\nInit Module Buffer: Addr: 0x%X, Side1: 0x%X, Side2: 0x%X, Side3: 0x%X, Side4: 0x%X, Side5: 0x%X, Side6: 0x%X\n", send_buffer.addr, send_buffer.neighbor_address[0], send_buffer.neighbor_address[1], send_buffer.neighbor_address[2], send_buffer.neighbor_address[3], send_buffer.neighbor_address[4], send_buffer.neighbor_address[5]);
        
    // send address to hub
    int success = i2c_write_timeout_us(i2c0, HUB_I2C_ADDRESS, (uint8_t*)(&send_buffer), sizeof(send_buffer), false, 100000);
    while(success == PICO_ERROR_GENERIC || success == PICO_ERROR_TIMEOUT) {
      success = i2c_write_timeout_us(i2c0, HUB_I2C_ADDRESS, (uint8_t*)(&send_buffer), sizeof(send_buffer), false, 100000);
    }

    // set up worker
    i2c_worker_init(i2c0, addr, _handler);
}

uint8_t I2C_Module::get_request_type() {
  return request_type_register;
}

void I2CModule::set_request_type(uint8_t request_id) {
  request_type_register = request_id;
}
