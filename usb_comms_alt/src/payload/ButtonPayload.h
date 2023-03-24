#pragma once

#include "IPayload.h"

class ButtonPayload: public IPayload {
    private:
    const button_data_t data;

    public:
    /**
     * construct a ButtonPayload with the given data
    */
    ButtonPayload(button_data_t _data): data(_data) {};

    ButtonPayload() = default;

    /**
     * retrieve the data in this IPayload as a payload_t
    */
    virtual payload_t toPayload() const;

    /**
     * retrieve the reportID for this payload type
    */
    virtual report_id_t reportID() const;

    /**
     * is the given IPayload equal to this ButtonPayload?
     * precondition: other is a ButtonPayload
    */
    virtual bool isEquivalent(const IPayload *other) const;
};