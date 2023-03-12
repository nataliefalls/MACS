#ifndef MULTICORE_REPORT_QUEUE_H
#define MULTICORE_REPORT_QUEUE_H

#include "IReportQueue.h"

class MulticoreReportQueue: public IReportQueue {
    private:
    /**
     * timeout for the queue requests in microseconds
    */
    static const uint64_t QUEUE_TIMEOUT = 1000;

    public:
    /**
     * push the given report to the queue.
     *  returns false on failure
     *  returns true on success
    */
    virtual bool queue_push(const report_t &report) const;

    /**
     * pop the first report off of the queue.
     *  returns false on failure
     *  returns true on success, in which case, the report argument
     *      contains the report that was popped
    */
    virtual bool queue_pop(report_t &report) const;
};

#endif