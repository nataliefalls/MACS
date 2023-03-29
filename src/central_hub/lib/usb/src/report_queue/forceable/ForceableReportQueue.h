#pragma once

#include "IForceableReportQueue.h"
#include "pico/util/queue.h"

/**
 * A report queue that allows you to "force" push reports.
*/
class ForceableReportQueue: public IForceableReportQueue {
    private:
    const bool queueOwner;
    
    protected:
    const IReportQueue *reportQueue;
    const uint32_t threshold;

    public:
    ForceableReportQueue() = default;

    ForceableReportQueue(queue_t *queue, uint32_t capacity);

    ForceableReportQueue(queue_t *queue);

    ForceableReportQueue(IReportQueue *_reportQueue);

    ~ForceableReportQueue();

    virtual bool forcePush(const report_t &report) const;
    
    virtual bool push(const report_t &report) const;

    virtual bool pop(report_t *report) const;

    virtual bool empty() const;

    virtual bool full() const;

    virtual uint32_t count() const;

    virtual uint32_t capacity() const;
};