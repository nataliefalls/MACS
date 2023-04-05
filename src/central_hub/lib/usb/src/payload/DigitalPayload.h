#pragma once

#include "IPayload.h"

class DigitalPayload: public IPayload {
    private:
    const digital_data_t data;

    public:
    /**
     * construct a DigitalPayload with the given data
    */
    DigitalPayload(digital_data_t _data): data(_data) {};

    DigitalPayload() = default;

    virtual payload_t toPayload() const;

    virtual report_id_t reportID() const;

    virtual bool isEquivalent(const IPayload *other) const;
};