#include "modules.h"
#include "report_types.h"
#include "usb_controller.h"

/**
 * MODULE
*/

Module::Module(ModuleID _moduleID, ModuleType _moduleType, module_coordinates_t _coordinates, USBController _controller) {
    this->moduleID = _moduleID;
    this->moduleType = _moduleType;
    this->coordinates = _coordinates;
    this->controller = _controller;
    // send module_connected_report
    this->controller.module_connected_report(this->moduleID, this->coordinates);
}

Module::~Module() {
    // send module_disconnected_report
    this->controller.module_disconnected_report(this->moduleID);
}

void Module::update(payload_t newState) {
    // only send the report if the new state is different from the old
    if (!this->sameState(this->state, newState)) {
        this->state = newState;
        this->sendReport(newState);
    }
}

void Module::sendReport(payload_t payload) {
    report_t report = {
        toReportID(this->moduleType),
        this->moduleID,
        payload
    };
    this->controller.input_report(report);
}

/**
 * BUTTON
*/

ButtonModule::ButtonModule(ModuleID moduleID, module_coordinates_t coordinates, USBController controller) {
    Module(moduleID, BUTTON, coordinates, controller);
}

bool ButtonModule::sameState(payload_t data1, payload_t data2) {
    return sameButtonData(data1.button, data2.button);
}

/**
 * DPAD
*/

DpadModule::DpadModule(ModuleID moduleID, module_coordinates_t coordinates, USBController controller) {
    Module(moduleID, DPAD, coordinates, controller);
}

bool DpadModule::sameState(payload_t data1, payload_t data2) {
    return sameDpadData(data1.dpad, data2.dpad);
}

/**
 * JOYSTICK
*/

JoystickModule::JoystickModule(ModuleID moduleID, module_coordinates_t coordinates, USBController controller) {
    Module(moduleID, DPAD, coordinates, controller);
}

bool JoystickModule::sameState(payload_t data1, payload_t data2) {
    return sameJoystickData(data1.joystick, data2.joystick);
}