#pragma once

#include "Payload.h"

class DigitalPayload: public Payload {
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

    protected:
    virtual bool sameData(const IPayload *other) const;
};