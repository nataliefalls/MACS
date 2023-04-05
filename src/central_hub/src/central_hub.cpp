#include <stdio.h>
#include <set>

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <pico/multicore.h>
#include "bsp/board.h"

#include <i2c_modules.h>
#include <Pwm.h>
#include <i2c_device.h>
#include "hub_utils.h"
#include <common.h>

#include "usbTask.h"
#include "usbUtil.h"
#include "ForceableReportQueue.h"

IForceableReportQueue *reportQueue;

void usbTask() {
    usbInit();
    usbMain(reportQueue);
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
    // initialize the shared queue between I2C and USB
    queue_t queue;
    reportQueue = new ForceableReportQueue(&queue);

    multicore_launch_core1(usbTask);

    /**
     * Somewhere on the I2C thread, a controller needs to be created, as such:
     * ReportQueueController *controller = new ReportQueueController(reportQueue);
     * 
     * This controller is used to create Module objects when they are connected:
     * Module<SomeKindOfPayload> module = new Module<SomeKindOfPayload>(moduleID, coordinates, controller);
     * 
     * This controller is the one and only interface for putting stuff on the queue, and
     * all of this is handled by the Module class. This is how to use the Module class:
     * 
     * Modules are updated as such:
     * datatype data = getDataFromModuleOverI2C(relevant, information);
     * // btw, SomeKindOfPayload inherits from the IPayload interface
     * SomeKindOfPayload *payload = new SomeKindOfPayload({ .someData = data });
     * if (!module->update(payload)) {
     *     delete payload;
     * }
     * 
     * When modules disconnect, modules should be deleted as such:
     * delete module;
     * 
     * That should be everything.
    */

    I2C_Hub hub(hub::QUEEN_SDA_PIN, hub::QUEEN_SCL_PIN, hub::WORKER_SDA_PIN, hub::WORKER_SCL_PIN);
    hub.setup();
    
    // while(1) {
    //     hub.i2c_task();
    // }

    // set pwm out pin
    // initialize pwm reader for pins

    // set up core 1 (usb)
}
