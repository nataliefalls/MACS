#pragma once

#include "report_ids.h"
#include "IPayload.h"
#include "IReportQueueController.h"

/**
 * Handles all state updates for a module.
 * - contains the state of a Module connected to the controller
 * - responsible for queuing reports based on updates
 * 
 * T is a subclass of IPayload, and represents the type of module / type of data it sends
*/
template <class T> class ModuleUpdateHandler {
    protected:
    const uint8_t moduleID;
    const IReportQueueController *controller;
    T *state;

    public:
    /**
     * construct a ModuleUpdateHandler with the given moduleID, and coordinates.
     * sends a module_connected_report
    */
    ModuleUpdateHandler(uint8_t _moduleID, module_coordinates_t _coordinates, IReportQueueController *_controller);

    ModuleUpdateHandler() = default;
    
    /**
     * destruct this module.
     * sends a module_disconnected_report
    */
    ~ModuleUpdateHandler();

    /**
     * update this module's state with the given data
     * sends a report if the new state is different from the old state
     * state is updated if and only if the report was queued successfully
     * returns true only if state is updated
    */
    bool update(T *newState);

    protected:
    bool sendReport(IPayload *newState) const;
};

// for implicit template instantiation
#include "ModuleUpdateHandlerImpl.h"