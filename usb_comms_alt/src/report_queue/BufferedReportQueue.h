#pragma once

#include "IReportQueue.h"

class BufferedReportQueue: public IReportQueue {
    private:
    // need to add a mutex field
    const uint8_t *buffer;
    const uint8_t size;

    public:
    /**
     * construct a buffered report queue with a buffer and its size
    */
    BufferedReportQueue(const uint8_t *_buffer, uint8_t _size)
        : buffer(_buffer), size(_size) {}

    BufferedReportQueue() = default;

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
};