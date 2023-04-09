#include <i2c_hub.h>
#include <vector>

#include <DigitalPayload.h>
#include <AnalogPayload.h>
#include <JoystickPayload.h>
#include <Pwm.h>

using namespace std::placeholders;


uint pwm_in[6] = {
  PWM_IN_SIDE_1,
  PWM_IN_SIDE_2,
  PWM_IN_SIDE_3,
  PWM_IN_SIDE_4,
  PWM_IN_SIDE_5,
  PWM_IN_SIDE_6
};
uint pwm_out = PWM_OUT_PIN;


/* I2C Hub Definitoin */

I2C_Hub::I2C_Hub(uint queen_sda, uint queen_scl, uint worker_sda, uint worker_scl,
                 ReportQueueController *controller,
                 std::function<void(i2c_inst_t *, i2c_worker_event_t)> handler)
    : I2C_Base(HUB_I2C_ADDRESS, worker_sda, worker_scl),
      _handler(handler),
      _controller(controller),
      _hub_pwm(pwm_out, pwm_in, 6) {
    this->queen_sda = queen_sda;
    this->queen_scl = queen_scl;
    // _controller = controller;
    


    _hub_pwm.setPWMOut((uint16_t)HUB_I2C_ADDRESS);
}

IPayload *payloadFromType(ModuleType type, uint8_t *buf) {
    // format buf into appropriate HID report payloads
    switch (type) {
    case kButton:
    case kSwitch:
        return new DigitalPayload({ buf[0] });
    case kSlider:
    case kPotentiometer:
        return new AnalogPayload({ buf[0] });
    case kJoystick:
        return new JoystickPayload({ buf[0], buf[1], buf[2] });
    default:
        return nullptr;
    }
}

void I2C_Hub::i2c_task() {
    std::vector<uint8_t> toRemove;
    for (auto mod : modules) {
        // retrieve address + module from map
        uint8_t address = mod.first;
        Module* this_module = mod.second;
        // Parse address for module type
        ModuleType type = parse_address(address);
        uint8_t *buf = new uint8_t[hw_size_from_type(type)];
        int count = i2c_read_blocking(i2c1, address, buf, hw_size_from_type(type), false);
        // if module is unresponsive, add it to a list to be removed later
        if (count == PICO_ERROR_GENERIC) {
            toRemove.push_back(address);
            //sleep_us(500000);
            continue;
        }
        // attempt to update the module with the new data
        IPayload *payload = payloadFromType(type, buf);
        if (!this_module->update(payload)) {
            delete payload;
        }

        delete[] buf;
    }
    // remove all the unresponsive addresses
    for (auto address : toRemove) {
        //printf("Device not recognized at address %#04X\n", address);
        Module *module = modules[address];
        if (module->disconnect()) {
            modules.erase(address);
            coordinates.erase(address);
            delete module;
        }
    }
}

void I2C_Hub::setup() {
    coordinates.insert({addr, {0, 0}});
    module_orientation.insert({addr, 0});
    initialize_i2c(i2c0, sda, scl);
    i2c_worker_init(i2c0, addr, _handler);
    initialize_i2c(i2c1, queen_sda, queen_scl);
}

bool I2C_Hub::i2c_handle(i2c_inst_t* i2c) {
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


    // if coordinates were not found, add this module as dependencies for its neighbors
    if(!have_coords) {
        for (uint8_t ii = 0; ii < 6; ii++) {
            coordinate_dependencies[received.neighbor_address[ii]]
                .push_back({received.addr, ii});
        }
    }
    return true;
}

void I2C_Hub::coordinate_helper(uint8_t address, uint8_t neighbor_side, uint8_t neighbor_address) {

    module_coordinates_t neighbor_coords = coordinates[neighbor_address];
    uint8_t u = neighbor_coords.u;
    uint8_t v = neighbor_coords.v;

    // find out what side the new module is connected on for the neighbor
    
    uint8_t address_request = address;
    uint8_t side;
    if (address_request == HUB_I2C_ADDRESS) {
	side = 6;
	for (uint8_t ii = 0; ii < 6; ii++) {
	  if (_hub_pwm.isConnected(ii) &&
	      (_hub_pwm.read_PW(ii) & 0x00FF) == address) {
	    side = ii;
	    break;
	    
	  }
	}
    }
    else {
      int success = i2c_write_blocking(i2c1, neighbor_address, &address_request, sizeof(address_request), true);

      uint8_t count = i2c_read_blocking(i2c1, address, &side, sizeof(side), true);

      if (count == PICO_ERROR_GENERIC) {
	//printf("Device not recognized at address %#04X\n", address);
	coordinates.erase(address);
	coordinate_dependencies.erase(address);
	delete modules[address];
	modules.erase(address);
      }
    }

    address_request = 0x0000;
    int success = i2c_write_blocking(i2c1, neighbor_address, &address_request, sizeof(address_request), false);
    
    //side = (neighbor_side + 3) % 6;
    uint8_t absolute_side = (side + module_orientation[neighbor_address]) % 6;

    module_coordinates_t new_coords;

    switch (absolute_side) {
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
    module_orientation[address] = ((int)(absolute_side) - (int)(neighbor_side))%6;
    // if modules does not contain this address, add coord_it
    ModuleType type = parse_address(address);
    Module* module = new Module(address, new_coords, _controller);
    module->connect();
    modules.insert({address, module});

<<<<<<< HEAD
}
=======
    std::map<uint8_t, std::vector<module_side>>::iterator depend_it;
    // if coordinates found, try to resolve other dependencies

    depend_it = coordinate_dependencies.find(address);
    if (depend_it != coordinate_dependencies.end()) {
        std::vector<module_side> depending_modules =
            coordinate_dependencies[address];

        for (module_side depender : depending_modules) {
            // find neighbors that don't have addresses yet
            if (coordinates.find(depender.module_addr) == coordinates.end()) {
                // assign them addresses
                coordinate_helper(depender.module_addr, depender.side, address);
            }
        }
        coordinate_dependencies[address].clear();
        coordinate_dependencies.erase(address);
    }
    
}
>>>>>>> CE
