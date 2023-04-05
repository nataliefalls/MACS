#pragma once

#include "IReportQueue.h"

/**
 * An interface for report queues that allow you to "force" push reports.
 * Under normal circumstances, when the queue is full, a push will fail,
 * and you would have to try to push that report again later. This interface
 * provides the `forcePush' method, which behaves like `push', except if the
 * queue is full, we attempt to push it on the next call to push.
 */
class IForceableReportQueue: public IReportQueue {
    public:
    /**
     * force the given report onto the queue.
     *  false return indicates the force failed, and the given report will be dropped
     *  true return indicates success
    */
    virtual bool forcePush(const report_t &report) const = 0;
};