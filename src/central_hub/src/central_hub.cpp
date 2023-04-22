#include <pico/multicore.h>
#include "bsp/board.h"

#include <hardware/pwm.h>
#include <Pwm.h>
#include <i2c_device.h>
#include <i2c_hub.h>
#include "hub_utils.h"

#include "IReportQueueController.h"
#include "ReportQueueSingleton.h"
#include "usbTask.h"

I2C_Hub *i2c_hub;

static void hub_worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_RECEIVE) {
        // only getting receives
        i2c_hub->i2c_handle(i2c);
    }
}

void usbMain() {
    usbInit();
    usbTask(ReportQueueSingleton::getHandler());
}

int main() {
    board_init();
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    uint pwm_in[6] = {
        PWM_IN_SIDE_1,
        PWM_IN_SIDE_2,
        PWM_IN_SIDE_3,
        PWM_IN_SIDE_4,
        PWM_IN_SIDE_5,
        PWM_IN_SIDE_6
    };
    uint pwm_out = PWM_OUT_PIN;

    Pwm module_pwm(pwm_out, pwm_in, 6);

    module_pwm.setPWMOut((uint16_t)HUB_I2C_ADDRESS);

    multicore_launch_core1(usbMain);

    i2c_hub = new I2C_Hub(
        hub::QUEEN_SDA_PIN,
        hub::QUEEN_SCL_PIN,
        hub::WORKER_SDA_PIN,
        hub::WORKER_SCL_PIN,
        ReportQueueSingleton::getController(),
        &hub_worker_callback);
    i2c_hub->setup();
    
    while(1) {
        i2c_hub->i2c_task();
    }
}
