#pragma once

#include <stdint.h>
#include <type_traits>
#include "Module.h"
#include "coordinates.h"

Module::Module(uint8_t _moduleID, module_coordinates_t coordinates, ReportQueueController *_controller)
    : moduleID(_moduleID), controller(_controller), state(nullptr) {
    // let the application know a module was connected
    this->controller->moduleConnectedReport(this->moduleID, coordinates);
}

Module::~Module() {
    // let the application know a module was disconnected
    this->controller->moduleDisconnectedReport(this->moduleID);
}

bool Module::update(IPayload *newState) {
    // only send the report if the new state is different from the old state
    if (newState->isEquivalent(this->state)) {
        return false;
    } else if (this->sendReport(newState)) {
        delete this->state;
        this->state = newState;
        return true;
    } else {
        return false;
    }
}

bool Module::sendReport(IPayload *newState) const {
    return this->controller->inputReport(
        this->moduleID,
        newState->reportID(),
        newState->toPayload()
    );
}