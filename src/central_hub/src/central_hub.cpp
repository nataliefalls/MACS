// #include <stdio.h>
#include <pico/multicore.h>
#include "bsp/board.h"

#include <i2c_hub.h>
#include <hardware/pwm.h>
#include <Pwm.h>
#include <i2c_device.h>
#include "hub_utils.h"

// #include "demo.h"
// #include "PicoQueueReportQueue.h"
#include "IReportQueueController.h"
#include "usbTask.h"
#include "ReportQueueFactory.h"

I2C_Hub *i2c_hub;

// IReportQueue *inputReportQueue;
// IReportQueue *connectionReportQueue;
// queue_t inputQueue;
// queue_t connectionQueue;

static void hub_worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_RECEIVE) {
        // only getting receives
        i2c_hub->i2c_handle(i2c);
    }
}

void usbMain() {
    usbInit();
    usbTask(ReportQueueFactory::getHandler());
    // usbTask(inputReportQueue, connectionReportQueue);
}

int main() {
    board_init();
    stdio_init_all();
    // init_adc_gpio();

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

    // inputReportQueue = new PicoQueueReportQueue(&inputQueue);
    // connectionReportQueue = new PicoQueueReportQueue(&connectionQueue);
    multicore_launch_core1(usbMain);

    // ReportQueueController *controller = new ReportQueueController(inputReportQueue, connectionReportQueue);

    // i2c_hub = new I2C_Hub(hub::QUEEN_SDA_PIN, hub::QUEEN_SCL_PIN, hub::WORKER_SDA_PIN, hub::WORKER_SCL_PIN, controller, &hub_worker_callback);
    i2c_hub = new I2C_Hub(
        hub::QUEEN_SDA_PIN,
        hub::QUEEN_SCL_PIN,
        hub::WORKER_SDA_PIN,
        hub::WORKER_SCL_PIN,
        ReportQueueFactory::getController(),
        &hub_worker_callback);
    i2c_hub->setup();
    
    while(1) {
        i2c_hub->i2c_task();
    }
}
