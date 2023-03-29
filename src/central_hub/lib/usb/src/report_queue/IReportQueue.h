#pragma once

#include "report_types.h"

class IReportQueue {
    public:
    /**
     * push the given report to the queue with a 1ms timeout.
     *  returns false on failure
     *  returns true on success
    */
    virtual bool push(const report_t &report) const = 0;

    /**
     * pop the first report off of the queue with a 1ms timeout.
     *  returns false on failure
     *  returns true on success, in which case, the report argument
     *      contains the report that was popped
    */
    virtual bool pop(report_t *report) const = 0;

    /**
     * is this queue empty?
    */
    virtual bool empty() const {
        return false;
    }

    /**
     * is this queue full?
    */
    virtual bool full() const {
        return false;
    }

    /**
     * how many reports are in this queue?
    */
    virtual uint32_t count() const {
        return 0;
    }

    /**
     * what is the total capacity of this queue?
    */
    virtual uint32_t capacity() const {
        return 0;
    }
};