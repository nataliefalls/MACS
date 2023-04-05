#include <stdio.h>
#include <pico/multicore.h>
#include "bsp/board.h"

#include <i2c_modules.h>
#include <hardware/pwm.h>
#include <Pwm.h>
#include "hub_utils.h"

#include "demo.h"
#include "PicoQueueReportQueue.h"
#include "ReportQueueController.h"
#include "usbTask.h"

IReportQueue *inputReportQueue;
IReportQueue *connectionReportQueue;
queue_t inputQueue;
queue_t connectionQueue;

void usbMain() {
    usbInit();
    usbTask(inputReportQueue, connectionReportQueue);
}

int main() {
    board_init();
    stdio_init_all();
    init_adc_gpio();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    I2C_Hub hub(hub::QUEEN_SDA_PIN, hub::QUEEN_SCL_PIN, hub::WORKER_SDA_PIN, hub::WORKER_SCL_PIN);
    hub.setup();

    inputReportQueue = new PicoQueueReportQueue(&inputQueue);
    connectionReportQueue = new PicoQueueReportQueue(&connectionQueue);
    multicore_launch_core1(usbMain);

    // usb core will handle the reports, so in this thread, we will send demo reports
    // we do this via a report queue controller
    ReportQueueController *controller = new ReportQueueController(inputReportQueue, connectionReportQueue);

    while (true) {
        queueDemoReport(controller);
    }
    
    // while(1) {
    //     hub.i2c_task();
    // }

    // set pwm out pin
    // initialize pwm reader for pins
}
