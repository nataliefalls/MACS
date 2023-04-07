#pragma once

#include <typeinfo>
#include "payload_types.h"
#include "report_ids.h"

class IPayload {
    public:
    /**
     * retrieve the data in this IPayload as a payload_t
    */
    virtual payload_t toPayload() const = 0;

    /**
     * retrieve the reportID for this payload type
    */
    virtual report_id_t reportID() const = 0;

    /**
     * is the given IPayload equivalent to this IPayload?
     * precondition: other is the same type as the instance invoking the function
    */
    virtual bool isEquivalent(const IPayload *other) const {
        return false;
    }

    /**
     * Is the given address valid for this payload type?
    */
    // virtual bool isValidAddress(uint8_t address) const = 0;
};