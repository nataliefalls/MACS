#ifndef I2C_MODULES
#define I2C_MODULES

// #include <pico/stdlib.h>
#include <i2c_base.h>

class I2C_Module : public I2C_Base {
public:
    I2C_Module(uint8_t address, uint8_t neighbor_address[],
	           uint sda, uint scl, ModuleType type,  std::function<void (i2c_inst_t *, i2c_worker_event_t)> handler);
    ~I2C_Module();
    // void worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event);
    void setup();
    void update_input(uint8_t* hardware_input);
    const uint8_t* get_status() const;
    uint8_t get_size();
protected:
private:
    uint8_t* hw_status;
    ModuleType hw_type;
    uint8_t hw_size;
    uint8_t* _neighbor_address;
    std::function<void (i2c_inst_t *, i2c_worker_event_t)> _handler;
};



#endif
