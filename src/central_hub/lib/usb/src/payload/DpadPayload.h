#pragma once

#include "IPayload.h"

class DpadPayload: public IPayload {
    private:
    const dpad_data_t data;

    public:
    /**
     * construct a DpadPayload with the given data
    */
    DpadPayload(dpad_data_t _data): data(_data) {};

    DpadPayload() = default;

    virtual payload_t toPayload() const;

    virtual report_id_t reportID() const;

    virtual bool isEquivalent(const IPayload *other) const;
};