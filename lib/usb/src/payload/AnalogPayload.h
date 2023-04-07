#pragma once

#include "IPayload.h"

class AnalogPayload: public IPayload {
    private:
    const analog_data_t data;

    public:
    /**
     * construct an analog payload with the given data
    */
    AnalogPayload(analog_data_t _data): data(_data) {};

    AnalogPayload() = default;

    virtual payload_t toPayload() const;

    virtual report_id_t reportID() const;

    virtual bool isEquivalent(const IPayload *other) const;
};