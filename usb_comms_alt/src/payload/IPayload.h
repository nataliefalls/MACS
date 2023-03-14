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

    bool operator==(const IPayload& other) const {
        if (&other == nullptr) {
            return this == nullptr;
        } else {
            return typeid(*this) == typeid(other) && isEqual(other);
        }
    }

    private:
    /**
     * is the given IPayload equal to this IPayload?
     * precondition: other is the same type as the instance invoking the function
    */
    virtual bool isEqual(const IPayload& other) const = 0;
};