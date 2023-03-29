#pragma once

#include "IForceableReportQueue.h"

class ForceableReportQueue: public IForceableReportQueue {
    protected:
    const IReportQueue *reportQueue;
    const IReportQueue *forcedReports;

    public:
    ForceableReportQueue() = default;

    ForceableReportQueue(IReportQueue *_reportQueue, IReportQueue *_forcedReports);

    ~ForceableReportQueue();

    /**
     * force the given report onto the queue.
     *  false return indicates the force failed, and the given report will be dropped
     *  true return indicates success
    */
    virtual bool forcePush(const report_t &report) const;
    
    /**
     * push the given report to the queue with a 1ms timeout.
     *  returns false on failure
     *  returns true on success
    */
    virtual bool push(const report_t &report) const;

    /**
     * pop the first report off of the queue with a 1ms timeout.
     *  returns false on failure
     *  returns true on success, in which case, the report argument
     *      contains the report that was popped
    */
    virtual bool pop(report_t *report) const;
};