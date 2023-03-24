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

    /**
     * retrieve the data in this IPayload as a payload_t
    */
    virtual payload_t toPayload() const;

    /**
     * retrieve the reportID for this payload type
    */
    virtual report_id_t reportID() const;

    /**
     * is the given IPayload equal to this DpadPayload?
     * precondition: other is a DpadPayload
    */
    virtual bool isEquivalent(const IPayload *other) const;
};