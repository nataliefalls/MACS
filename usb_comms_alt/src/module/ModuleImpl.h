#pragma once

#include <type_traits>
#include "Module.h"
#include "report_types.h"

template <class T>
Module<T>::Module(uint8_t _moduleID, module_coordinates_t coordinates, ReportQueueController *_controller)
    : moduleID(_moduleID), controller(_controller), state(nullptr) {
    // verify at compile-time that T is a subclass of IPayload
    static_assert(std::is_base_of<IPayload, T>::value, "template parameter not a subclass of IPayload");
    // let the application know a module was connected
    this->controller->moduleConnectedReport(this->moduleID, coordinates);
}

template <class T>
Module<T>::~Module() {
    // let the application know a module was disconnected
    this->controller->moduleDisconnectedReport(this->moduleID);
}

template <class T>
void Module<T>::update(T *newState) {
    // only send the report if the new state:
    // - has the correct payload type
    // - is different from the old
    if (this->state == newState) {
        return;
    } else if (this->sendReport(newState)) {
        this->state = newState;
    }
}

template <class T>
bool Module<T>::sendReport(IPayload *newState) const {
    return this->controller->inputReport(
        this->moduleID,
        newState->reportID(),
        newState->toPayload()
    );
}