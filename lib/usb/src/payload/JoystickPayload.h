#pragma once

#include "IPayload.h"

class JoystickPayload: public IPayload {
    private:
    const joystick_data_t data;

    static const uint8_t TOLERANCE = 4;

    public:
    /**
     * construct a JoystickPayload with the given data
    */
    JoystickPayload(joystick_data_t _data): data(_data) {};

    JoystickPayload() = default;

    virtual payload_t toPayload() const;

    virtual report_id_t reportID() const;

    virtual bool isEquivalent(const IPayload *other) const;

    private:
    bool isWithinTolerance(uint8_t value1, uint8_t value2) const;
};