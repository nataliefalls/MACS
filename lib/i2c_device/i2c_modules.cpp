#include <cstdio>

#include "i2c_modules.h"
#include <DigitalPayload.h>
#include <AnalogPayload.h>
#include <JoystickPayload.h>

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
		       uint sda, uint scl, ModuleType type)
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

I2C_Hub::I2C_Hub(uint queen_sda, uint queen_scl, uint worker_sda, uint worker_scl, ReportQueueController *controller) 
        : I2C_Base(HUB_I2C_ADDRESS, worker_sda, worker_scl) {
    this->queen_sda = queen_sda;
    this->queen_scl = queen_scl;
    _controller = controller;
}

void I2C_Hub::i2c_task(/* queue to USB task */) {
    for (auto mod : modules) {

        uint8_t address = mod.first;
        Module* this_module = mod.second;
        
        // Parse address for module type
        ModuleType type = parse_address(address);

        int count;
        uint8_t *buf;

        count = i2c_read_blocking(i2c1, address, buf, hw_size_from_type(type), false);

        IPayload* payload;

        // format buf into appropriate HID reports and queue them into USB object
        switch (type)
        {
        case kButton:
        case kSwitch:
            payload = new DigitalPayload({ buf[0] });
            if (!this_module->update(payload)) {
                delete payload;
            }
            break;

        case kSlider:
        case kPotentiometer:
            payload = new AnalogPayload({ buf[0] });
            if (!this_module->update(payload)) {
                delete payload;
            }
            break;

        case kJoystick:
            payload = new JoystickPayload({ buf[0], buf[1], buf[2] });
            if (!this_module->update(payload)) {
                delete payload;
            }
            break;

        default:
            break;
        }

        if (count == PICO_ERROR_GENERIC) {
            //printf("Device not recognized at address %#04X\n", address);
            delete modules[address];
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
        i2c_read_raw_blocking(i2c, (uint8_t *)(&received), sizeof(received));

        bool have_coords = false;
        // check to see if neighboring side has coordinates
        std::map<uint8_t, module_coordinates_t>::iterator coord_it;

        for (int ii = 0; ii < 6; ii++) {
            coord_it = coordinates.find(received.neighbor_address[ii]);
            if (coord_it != coordinates.end()) {
                have_coords = true;
                // if coord_it does, find the coordinates of the new module
                coordinate_helper(received.addr, ii, received.neighbor_address[ii]);
                break;
            }
        }

        std::map<uint8_t, std::vector<module_side>>::iterator depend_it;
        // if coordinates found, try to resolve other dependencies
        if (have_coords) {
            depend_it = coordinate_dependencies.find(received.addr);
            if (depend_it != coordinate_dependencies.end()) {
                std::vector<module_side> depending_modules =
                    coordinate_dependencies[received.addr];

                for (module_side depender : depending_modules) {
                    // find neighbors that don't have addresses yet
                    if (coordinates.find(depender.module_addr) == coordinates.end()) {
                        // assign them addresses
                        coordinate_helper(depender.module_addr, depender.side, received.addr);
                    }
                }
            }
        }
        // if coordinates were not found, add this module as dependencies for its neighbors
        else {
            for (uint8_t ii = 0; ii < 6; ii++) {
                coordinate_dependencies[received.neighbor_address[ii]]
                    .push_back({received.addr, ii});
            }
        }
    }
}

void I2C_Hub::coordinate_helper(uint8_t address, uint8_t neighbor_side, uint8_t neighbor_address) {

    module_coordinates_t neighbor_coords = coordinates[neighbor_address];
    uint8_t u = neighbor_coords.u;
    uint8_t v = neighbor_coords.v;
    // find out what side the new module is connected on for the neighbor
    uint8_t side = (neighbor_side + 3) % 6;

    module_coordinates_t new_coords;

    switch (side) {
    case 0:
        new_coords = {(uint8_t)(u - 1), (uint8_t)(v + 1)};
        break;
    case 1:
        new_coords = {u, (uint8_t)(v + 1)};
        break;
    case 2:
        new_coords = {(uint8_t)(u + 1), v};
        break;
    case 3:
        new_coords = {uint8_t(u + 1), (uint8_t)(v - 1)};
        break;
    case 4:
        new_coords = {u, (uint8_t)(v - 1)};
        break;
    case 5:
        new_coords = {(uint8_t)(u - 1), v};
        break;
    }

    coordinates[address] = new_coords;
    // if modules does not contain this address, add coord_it
    // auto success = modules.insert(address);
    // TODO: create module class here that does the USB stuff
    ModuleType type = parse_address(address);
    Module* module;
    switch (type)
    {
    case kButton:
    case kSwitch:
        module = new Module(address, new_coords, _controller);
        modules.insert({address, module});
        break;
    
    case kSlider:
    case kPotentiometer:
        module = new Module(address, new_coords, _controller);
        modules.insert({address, module});
        break;
    
    case kJoystick:
        module = new Module(address, new_coords, _controller);
        modules.insert({address, module});
        break;
    
    default:
        break;
    }

}
