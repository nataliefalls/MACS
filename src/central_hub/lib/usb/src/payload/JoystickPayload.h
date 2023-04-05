#pragma once

#include "IPayload.h"

class JoystickPayload: public IPayload {
    protected:
    const joystick_data_t data;

    private:
    static const uint8_t TOLERANCE = 1;

    public:
    /**
     * construct a JoystickPayload with the given data
    */
    JoystickPayload(joystick_data_t _data): data(_data) {};

    JoystickPayload() = default;

    /**
     * retrieve the data in this IPayload as a payload_t
    */
    virtual payload_t toPayload() const;

    /**
     * retrieve the reportID for this payload type
    */
    virtual report_id_t reportID() const;

    /**
     * is the given IPayload equal to this JoystickPayload?
     * precondition: other is a JoystickPayload
    */
    virtual bool isEquivalent(const IPayload *other) const;

    private:
    bool isWithinTolerance(uint8_t x, uint8_t y) const;
};