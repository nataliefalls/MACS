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

    virtual bool queuePush(const report_t &report) const;

    virtual bool queuePop(report_t *report) const;

    virtual bool queuePeek(report_t *report) const;
};