#ifndef REPORT_TYPES_H
#define REPORT_TYPES_H

#include "tusb.h"
#include "report_ids.h"
#include "payload_types.h"

typedef uint8_t ModuleID;

enum ModuleType {
    BUTTON = REPORT_ID_BUTTON_DATA,
    DPAD,
    JOYSTICK
};

report_id_t toReportID(ModuleType moduleType) {
    switch (moduleType) {
        case BUTTON:
            return REPORT_ID_BUTTON_DATA;
        case DPAD:
            return REPORT_ID_DPAD_DATA;
        case JOYSTICK:
            return REPORT_ID_JOYSTICK_DATA;
        default:
            // this is the least bad default case, since duplicate
            // module_connected messages are ignored
            return REPORT_ID_MODULE_CONNECTED;
    }
}

class Module {
    protected:
    ModuleID moduleID;
    ModuleType moduleType;
    payload_t state;
    module_coordinates_t coordinates;
    USBController controller;

    public:
    /**
     * construct a module with the given moduleID, moduleType, and coordinates.
     * sends a module_connected_report
    */
    Module(ModuleID _moduleID, ModuleType _moduleType, module_coordinates_t _coordinates, USBController _controller);

    Module() = default;
    
    /**
     * destruct a module with the given moduleID and moduleType.
     * sends a module_disconnected_report
    */
    ~Module();

    /**
     * update this module's state with the given data
    */
    void update(payload_t newState);

    protected:
    virtual bool sameState(payload_t data1, payload_t data2);
    void sendReport(payload_t payload);
};

class ButtonModule: public Module {
    public:
    /**
     * construct a button module with the given moduleID.
     * sends a module_connected_report
    */
    ButtonModule(ModuleID moduleID, module_coordinates_t coordinates, USBController controller);

    ButtonModule() = default;

    protected:
    virtual bool sameState(payload_t data1, payload_t data2);
};

class DpadModule: public Module {
    public:
    /**
     * construct a dpad module with the given moduleID.
     * sends a module_connected_report
    */
    DpadModule(ModuleID _moduleID, module_coordinates_t coordinates, USBController controller);

    DpadModule() = default;

    protected:
    virtual bool sameState(payload_t data1, payload_t data2);
};

class JoystickModule: public Module {
    public:
    /**
     * construct a joystick module with the given moduleID.
     * sends a module_connected_report
    */
    JoystickModule(ModuleID _moduleID, module_coordinates_t coordinates, USBController controller);

    JoystickModule() = default;

    protected:
    virtual bool sameState(payload_t data1, payload_t data2);
};

#endif