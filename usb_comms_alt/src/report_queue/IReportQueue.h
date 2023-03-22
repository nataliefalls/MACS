#pragma once

#include "report_types.h"

class IReportQueue {
    public:
    /**
     * push the given report to the queue with a 1ms timeout.
     *  returns false on failure
     *  returns true on success
    */
    virtual bool queue_push(const report_t &report) const = 0;

    /**
     * pop the first report off of the queue with a 1ms timeout.
     *  returns false on failure
     *  returns true on success, in which case, the report argument
     *      contains the report that was popped
    */
    virtual bool queue_pop(report_t *report) const = 0;
};