#include <cstdio>

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

I2C_Module::I2C_Module(uint8_t address, uint8_t neighbor_address[],
		       uint sda, uint scl, Module type)
  : I2C_Base(address, sda, scl),
     _neighbor_address(neighbor_address) {
    hw_type = type;
    hw_size = hw_size_from_type(hw_type);
    hw_status = new uint8_t[hw_size];
}

I2C_Module::~I2C_Module() {
    delete hw_status;
}

void I2C_Module::worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_REQUEST) {
        //printf("Worker: Received request for val from queen\n");
        i2c_write_raw_blocking(i2c, hw_status, hw_size_from_type(hw_type));
    }
}

void I2C_Module::update_input(uint8_t* hardware_input) {
    // Data is copied to disallow input functions from mutating a private I2C_Module data member
    std::copy(hardware_input, hardware_input + hw_size, hw_status);
    for (int i = 0; i < hw_size; i++) {
        printf("%d ", hw_status[i]);
    }
}

void I2C_Module::setup() {
    initialize_i2c(i2c0, sda, scl);

    // create buffer containing address and neighbor data to send to hub
    init_module_buffer send_buffer;
    send_buffer.addr = addr;
    for (int ii = 0; ii < 6; ii++) {
      send_buffer.neighbor_address[ii] = _neighbor_address[ii];
    }
        
    // send address to hub
    int success = i2c_write_timeout_us(i2c0, HUB_I2C_ADDRESS, (uint8_t*)(&send_buffer), sizeof(send_buffer), false, 100000);
    while(success == PICO_ERROR_GENERIC || success == PICO_ERROR_TIMEOUT) {
      success = i2c_write_timeout_us(i2c0, HUB_I2C_ADDRESS, (uint8_t*)(&send_buffer), sizeof(send_buffer), false, 100000);
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
        init_module_buffer received;
        i2c_read_raw_blocking(i2c, (uint8_t*)(&received), sizeof(received));

	module_coordinates_t new_coords;
	// check to see if neighboring side has coordinates
	std::map<uint8_t, module_coordinates_t>::iterator it;

	for (int ii = 0; ii < 6; ii++) {
	  it = coordinates.find(received.neighbor_address[ii]);
	  if (it != coordinates.end()) {
	    // if it does, find the coordinates of the new module
	    new_coords = coordinate_helper(ii, received.neighbor_address[ii]);
	    break;
	  }
	}
	

	
        // if modules does not contain this address, add it
        auto success = modules.insert(received.addr);
    }
}

module_coordinates_t I2C_Hub::coordinate_helper(uint8_t neighbor_side, uint8_t neighbor_address) {

  module_coordinates_t neighbor_coords = coordinates[neighbor_address];
  uint8_t u = neighbor_coords.u;
  uint8_t v = neighbor_coords.v;
  // find out what side the new module is connected on for the neighbor
  uint8_t side = (neighbor_side + 3) % 6;
  switch (side) {
  case 0:
    return {u - 1, v + 1};
    break;
  case 1:
    return {u, v + 1};
    break;
  case 2:
    return {u + 1, v};
    break;
  case 3:
    return {u + 1, v - 1};
    break;
  case 4:
    return {u, v - 1};
    break;
  case 5:
    return {u - 1, v};
    break;
  }
  
}
