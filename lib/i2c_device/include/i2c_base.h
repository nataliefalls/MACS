#ifndef I2C_BASE
#define I2C_BASE

#include <pico/stdlib.h>
#include <i2c_device.h>
#include <common.h>

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

static inline uint8_t hw_size_from_type(ModuleType type) {
    switch (type)
    {
    case kButton:
    case kSwitch:
    case kSlider:
    case kPotentiometer:
        return 1;
    case kJoystick:
        return 3;
    default:
        return -1;
    }
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
        uint8_t neighbor_address[6];
    };
};

#endif