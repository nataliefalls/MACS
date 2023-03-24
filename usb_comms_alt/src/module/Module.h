#pragma once

#include "report_ids.h"
#include "IPayload.h"
#include "ReportQueueController.h"

/**
 * Parameter T is a subclass of IPayload, and represents the type of module
 * and the type of data it sends.
*/
template <class T> class Module {
    protected:
    const uint8_t moduleID;
    const ReportQueueController *controller;
    T *state;

    public:
    /**
     * construct a module with the given moduleID, and coordinates.
     * sends a module_connected_report
    */
    Module(uint8_t _moduleID, module_coordinates_t _coordinates, ReportQueueController *_controller);

    Module() = default;
    
    /**
     * destruct a module with the given moduleID and moduleType.
     * sends a module_disconnected_report
    */
    ~Module();

    /**
     * update this module's state with the given data
     * sends a report if one of the following is true:
     * - the new state is different from the old state
     * - we force a send
     * state is updated iff the report was queued successfully
    */
    void update(T *newState);

    protected:
    bool sendReport(IPayload *newState) const;
};

// for implicit template instantiation
#include "ModuleImpl.h"