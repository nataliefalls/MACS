#include <i2c_base.h>
#include <set>
#include <map>
#include <coordinates.h>
#include <ReportQueueController.h>
#include <Module.h>

class I2C_Hub : public I2C_Base {
    // contains worker and queen running simultaneously
public:
    I2C_Hub(uint queen_sda, uint queen_scl, uint worker_sda, uint worker_scl, ReportQueueController *controller, std::function<void (i2c_inst_t *, i2c_worker_event_t)> handler);
    void i2c_task();
    void setup();
    //void worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event);
    bool i2c_handle(i2c_inst_t* i2c);
protected:
private:
    
    struct module_side {
        uint8_t module_addr;
        uint8_t side;
    };

    void coordinate_helper(uint8_t address, uint8_t neighbor_side, uint8_t neighbor_address);
  
    uint queen_sda;
    uint queen_scl;
    // std::set<uint8_t> modules;
    std::map<uint8_t, Module*> modules;
    std::map<uint8_t, module_coordinates_t> coordinates;
    std::map<uint8_t, std::vector<module_side>> coordinate_dependencies;
    ReportQueueController* _controller;
    std::function<void (i2c_inst_t *, i2c_worker_event_t)> _handler;
};