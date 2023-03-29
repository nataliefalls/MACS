#pragma once

#include "IReportQueue.h"

class IForceableReportQueue: public IReportQueue {
    public:
    /**
     * force the given report onto the queue.
     *  false return indicates the force failed, and the given report will be dropped
     *  true return indicates success
    */
    virtual bool forcePush(const report_t &report) const = 0;
};