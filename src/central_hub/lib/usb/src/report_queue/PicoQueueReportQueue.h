#pragma once

#include "IReportQueue.h"
#include "pico/util/queue.h"

class PicoQueueReportQueue: public IReportQueue {
    private:
    /**
     * timeout for the queue requests in microseconds
    */
    static const uint64_t QUEUE_TIMEOUT = 1000;

    queue_t *queue;

    bool init;

    public:
    /**
     * constructs a PicoQueueReportQueue with the given queue_t
    */
    PicoQueueReportQueue(queue_t *_queue);

    /**
     * default constructor
    */
    PicoQueueReportQueue() = default;

    /**
     * destruct a PicoQueueReportQueue
    */
    ~PicoQueueReportQueue();

    virtual bool queuePush(const report_t &report) const;

    virtual bool queuePop(report_t *report) const;

    virtual bool queuePeek(report_t *report) const;

    protected:
    /**
     * check if a timeout has occured
    */
    bool timeout();
};