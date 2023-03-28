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
    virtual bool queue_pop(report_t *report) const;

    protected:
    /**
     * check if a timeout has occured
    */
    bool timeout();
};