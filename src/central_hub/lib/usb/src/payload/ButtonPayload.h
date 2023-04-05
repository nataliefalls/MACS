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

    virtual payload_t toPayload() const;

    virtual report_id_t reportID() const;

    virtual bool isEquivalent(const IPayload *other) const;
};