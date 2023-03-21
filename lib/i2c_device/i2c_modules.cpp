#include <functional>

#include "i2c_modules.h"

using namespace std::placeholders;

/* I2C Base Definition */

I2C_Base::I2C_Base(uint8_t addr, uint sda, uint scl) {
    this->addr = addr;
    this->sda = sda;
    this->scl = scl;
}

I2C_Base::~I2C_Base() {

}

/* I2C Module Definition */

I2C_Module::I2C_Module(uint address, uint sda, uint scl, Module type) : I2C_Base(address, sda, scl) {
    hw_type = type;
}

void I2C_Module::worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_REQUEST) {
        //printf("Worker: Received request for val from queen\n");
        i2c_write_raw_blocking(i2c, hw_status, hw_size_from_type(hw_type));
    }
}

// Hardware Input must be the correct size corresponding with the type of module
void I2C_Module::update_input(uint8_t* hardware_input, Module type) {
    hw_status = hardware_input;
    hw_type = type;
}

void I2C_Module::setup() {
    initialize_i2c(i2c0, sda, scl);

    // send address to hub
    int success = i2c_write_timeout_us(i2c0, HUB_I2C_ADDRESS, &addr, sizeof(addr), false, 100000);
    while(success == PICO_ERROR_GENERIC || success == PICO_ERROR_TIMEOUT) {
        success = i2c_write_timeout_us(i2c0, HUB_I2C_ADDRESS, &addr, sizeof(addr), false, 100000);
    }

    // set up worker
    i2c_worker_init(i2c0, addr, std::bind(&I2C_Module::worker_callback, this, _1, _2));
}

/* I2C Hub Definitoin */

I2C_Hub::I2C_Hub(uint queen_sda, uint queen_scl, uint worker_sda, uint worker_scl) 
        : I2C_Base(HUB_I2C_ADDRESS, worker_sda, worker_scl) {
    this->queen_sda = queen_sda;
    this->queen_scl = queen_scl;
}

void I2C_Hub::i2c_task(/* queue to USB task */) {
    for (uint8_t address : modules) {

        // Parse address for module type
        Module type = parse_address(address);

        int count;
        uint8_t *buf;

        count = i2c_read_blocking(i2c1, address, buf, hw_size_from_type(type), false);

        // format buf into appropriate HID reports and queue them into USB object
        switch (type)
        {
        case kButton:
            
            break;
        case kSwitch:
            
            break;
        
        case kJoystick:
            
            break;

        case kSlider:
            break;
        case kPotentiometer:
            
            break;
        
        default:
            break;
        }

        if (count == PICO_ERROR_GENERIC) {
            //printf("Device not recognized at address %#04X\n", address);
            modules.erase(address);
            continue;
        }


        // do stuff with read buffer here? or in case block?
    }
}

void I2C_Hub::setup() {
    initialize_i2c(i2c0, sda, scl);
    i2c_worker_init(i2c0, addr, std::bind(&I2C_Hub::worker_callback, this, _1, _2));
    initialize_i2c(i2c1, queen_sda, queen_scl);
}

void I2C_Hub::worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_RECEIVE) {
        // only getting receives
        uint8_t received;
        i2c_read_raw_blocking(i2c, &received, 1);
        // if modules does not contain this address, add it
        auto success = modules.insert(received);
    }
}