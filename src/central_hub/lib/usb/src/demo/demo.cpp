#include "demo.h"
#include "ForceableReportQueueController.h"
#include "ReportQueueHandler.h"
#include "ForceableReportQueue.h"

#include "gpioUtils.h"
#include "demoUtils.h"

ModuleUpdateHandler<ButtonPayload> *buttonModule;
ModuleUpdateHandler<JoystickPayload> *joystickModule;

bool queue_button_report(IForceableReportQueueController *controller) {
    static bool buttonConnected = false;
    return toggleButtonConnection(&buttonModule, controller, buttonConnected)
        || (buttonConnected && updateButtonModule(buttonModule));
}

bool queue_joystick_report(IForceableReportQueueController *controller) {
    static bool joystickConnected = false;
    return toggleJoystickConnection(&joystickModule, controller, joystickConnected)
        || (joystickConnected && updateJoystickModule(joystickModule));
}

bool queue_demo_report(IForceableReportQueueController *controller) {
    static bool joystickOrButton = true;
    bool ret = false;

    if (joystickOrButton) {
        ret = queue_joystick_report(controller);
    } else {
        ret = queue_button_report(controller);
    }

    joystickOrButton = !joystickOrButton;
    return ret;
}