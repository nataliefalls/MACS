#pragma once

#include "IReportQueue.h"

class MulticoreReportQueue: public IReportQueue {
    private:
    /**
     * timeout for the queue requests in microseconds
    */
    static const uint64_t QUEUE_TIMEOUT = 1000;

    public:
    virtual bool queuePush(const report_t &report) const;

    virtual bool queuePop(report_t *report) const;

    virtual bool queuePeek(report_t *report) const;
};