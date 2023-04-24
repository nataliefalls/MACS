#pragma once 

#include "IPayload.h"

class Payload: public IPayload {
    public:
    virtual bool isEquivalent(const IPayload *other) const final;

    protected:
    /**
     * does the given payload have the same data as this payload?
     * precondition: other is the same type as the instance invoking the function
    */
    virtual bool sameData(const IPayload *other) const = 0;
};