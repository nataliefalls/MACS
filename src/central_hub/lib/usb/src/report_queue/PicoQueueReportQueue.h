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

    public:
    /**
     * constructs a PicoQueueReportQueue with the given queue_t
    */
    PicoQueueReportQueue(queue_t *_queue);
    
    /**
     * constructs a PicoQueueReportQueue with the given queue_t and capacity
    */
    PicoQueueReportQueue(queue_t *_queue, uint32_t capacity);

    /**
     * default constructor
    */
    PicoQueueReportQueue() = default;

    /**
     * destruct a PicoQueueReportQueue
    */
    ~PicoQueueReportQueue();

    virtual bool push(const report_t &report) const;

    virtual bool pop(report_t *report) const;

    virtual bool empty() const;

    virtual bool full() const;

    virtual uint32_t count() const;

    virtual uint32_t capacity() const;
};