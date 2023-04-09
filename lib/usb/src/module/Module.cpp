#include <stdint.h>
#include <type_traits>
#include "Module.h"
#include "coordinates.h"

Module::Module(uint8_t _moduleID, module_coordinates_t _coordinates, ReportQueueController *_controller)
    : moduleID(_moduleID), controller(_controller), coordinates(_coordinates), connected(false), state(nullptr) {}

Module::~Module() {
    if (this->state) delete this->state;
}

bool Module::connect() {
    if (!this->connected) {
        this->connected = this->controller->moduleConnectedReport(this->moduleID, coordinates);
    }
    return this->connected;
}

bool Module::disconnect() {
    if (this->connected) {
        this->connected = !this->controller->moduleDisconnectedReport(this->moduleID);
    }
    return !this->connected;
}

bool Module::update(IPayload *newState) {
    if (newState == nullptr) return false;
    // only send the report if the new state is different from the old state
    if (newState->isEquivalent(this->state)) {
        return false;
    } else if (this->sendReport(newState)) {
        if (this->state != nullptr) delete this->state;
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