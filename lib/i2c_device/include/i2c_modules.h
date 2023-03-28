#ifndef I2C_MODULES
#define I2C_MODULES

#include <pico/stdlib.h>
#include <i2c_device.h>
#include <common.h>
#include <set>

/* Utility functions */

static inline void initialize_i2c(i2c_inst_t* i2c, uint sda, uint scl) {

    gpio_init(sda);
    gpio_init(scl);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    i2c_init(i2c, I2C_BAUDRATE);
}

static inline uint8_t hw_size_from_type(Module type) {
    switch (type)
    {
    case kButton:
    case kSwitch:
        return 1;
    case kSlider:
    case kPotentiometer:
        return 2;
    case kJoystick:
        return 3;
    default:
        return -1;
    }
}

static inline i2c_inst_t* i2c_block_from_gpio(uint pin) {
//invalid_params_if(I2C, pin != 26 && pin != 27 && pin >= 22);
//if (pin < 22) {
//    return i2c_get_instance((pin / 2) % 2);
//} else if (pin == 26 || pin == 27) {
//    return i2c_get_instance(1);
//}
  return NULL;
}

class I2C_Base {
public:
    I2C_Base(uint8_t addr, uint sda, uint scl);
    ~I2C_Base();
    virtual void setup() = 0;
protected:

    uint8_t addr;
    uint sda;
    uint scl;

  struct init_module_buffer {
    uint8_t addr;
    uint8_t neighbor_address;
    uint8_t neighbor_side;
  };
};

class I2C_Module : public I2C_Base {
public:
    I2C_Module(uint8_t address, uint8_t neighbor_side, uint8_t neighbor_address,
	       uint sda, uint scl, Module type);
    void worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event);
    void setup();
    void update_input(uint8_t* hardware_input, Module type);
protected:
private:
    uint8_t* hw_status;
    Module hw_type;
    uint8_t _neighbor_side;
    uint8_t _neighbor_address;
};

class I2C_Hub : public I2C_Base {
    // contains worker and queen running simultaneously
public:
    I2C_Hub(uint queen_sda, uint queen_scl, uint worker_sda, uint worker_scl);
    void i2c_task(/* Pass queue to USB thread here */);
    void setup();
    void worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event);
protected:
private:
    uint queen_sda;
    uint queen_scl;
    std::set<uint8_t> modules;
};

#endif
