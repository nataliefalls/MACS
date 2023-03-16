#include <pico/stdlib.h>
#include "i2c_device.h"
#include <common.h>
#include <set>

class I2C_Base {
public:
    I2C_Base(uint8_t addr, uint sda, uint scl);
    ~I2C_Base();
    virtual void setup() = 0;
protected:

    uint8_t addr;
    uint sda;
    uint scl;
};

class I2C_Module : public I2C_Base {
public:
    I2C_Module(uint address, uint sda, uint scl, Module type);
    void worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event);
    void setup();
    void update_input(uint8_t* hardware_input, Module type);
protected:
private:
    uint8_t* hw_status;
    Module hw_type;
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