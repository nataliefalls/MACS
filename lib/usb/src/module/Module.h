#pragma once

#include "report_ids.h"
#include "IPayload.h"
#include "ReportQueueController.h"

/**
 * Handles all state updates for a module.
 * - contains the state of a Module connected to the controller
 * - responsible for queuing reports based on updates
*/
class Module {
    protected:
    bool connected;
    const uint8_t moduleID;
    const module_coordinates_t coordinates;
    const ReportQueueController *controller;
    IPayload *state;

    public:
    /**
     * construct a ModuleUpdateHandler with the given moduleID, and coordinates.
    */
    Module(uint8_t _moduleID, module_coordinates_t _coordinates, ReportQueueController *_controller);

    Module() = default;
    
    /**
     * destruct this module.
     * sends a module_disconnected_report
    */
    ~Module();

    /**
     * send a module connected report.
     * 
     * if module is already connected, this function immediately returns true
     * without sending the report.
    */
    bool connect();

    /**
     * send a module disconnected report.
     * 
     * if module is already disconnected, this function immediately returns true
     * without sending the report.
    */
    bool disconnect();

    /**
     * update this module's state with the given data
     * sends a report if the new state is different from the old state
     * state is updated if and only if the report was queued successfully
     * returns true only if state is updated
    */
    bool update(IPayload *newState);

    protected:
    bool sendReport(IPayload *newState) const;
};