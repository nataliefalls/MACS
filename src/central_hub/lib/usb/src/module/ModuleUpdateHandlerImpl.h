#pragma once

#include <stdint.h>
#include <type_traits>
#include "ModuleUpdateHandler.h"
#include "coordinates.h"

template <class T>
ModuleUpdateHandler<T>::ModuleUpdateHandler(uint8_t _moduleID, module_coordinates_t coordinates, IReportQueueController *_controller)
    : moduleID(_moduleID), controller(_controller), state(nullptr) {
    // verify at compile-time that T is a subclass of IPayload
    static_assert(std::is_base_of<IPayload, T>::value, "template parameter not a subclass of IPayload");
    // let the application know a module was connected
    this->controller->moduleConnectedReport(this->moduleID, coordinates);
}

template <class T>
ModuleUpdateHandler<T>::~ModuleUpdateHandler() {
    // let the application know a module was disconnected
    this->controller->moduleDisconnectedReport(this->moduleID);
}

template <class T>
bool ModuleUpdateHandler<T>::update(T *newState) {
    // only send the report if the new state is different from the old state
    //      note: the following call meets the precondition, since both this->state
    //      and newState are of type T.
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

template <class T>
bool ModuleUpdateHandler<T>::sendReport(IPayload *newState) const {
    return this->controller->inputReport(
        this->moduleID,
        newState->reportID(),
        newState->toPayload()
    );
}