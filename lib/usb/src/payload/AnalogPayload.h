#pragma once

#include "Payload.h"

class AnalogPayload: public Payload {
    private:
    const analog_data_t data;

    static const uint8_t TOLERANCE = 4;

    public:
    /**
     * construct an analog payload with the given data
    */
    AnalogPayload(analog_data_t _data): data(_data) {};

    AnalogPayload() = default;

    virtual payload_t toPayload() const;

    virtual report_id_t reportID() const;

    protected:
    virtual bool sameData(const IPayload *other) const;

    private:
    bool isWithinTolerance(uint8_t value1, uint8_t value2) const;
};