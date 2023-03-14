#pragma once

#include "IPayload.h"

class JoystickPayload: public IPayload {
    private:
    const joystick_data_t data;

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

    private:
    /**
     * is the given IPayload equal to this JoystickPayload?
     * precondition: other is a JoystickPayload
    */
    virtual bool isEqual(const IPayload& other) const;
};