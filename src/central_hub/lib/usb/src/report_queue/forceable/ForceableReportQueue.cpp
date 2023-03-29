#include "ForceableReportQueue.h"
#include "PicoQueueReportQueue.h"

ForceableReportQueue::ForceableReportQueue(queue_t *queue, uint32_t capacity):
    threshold(capacity), queueOwner(true) {
    this->reportQueue = new PicoQueueReportQueue(queue, capacity * 2);
}

ForceableReportQueue::ForceableReportQueue(IReportQueue *_reportQueue):
    reportQueue(_reportQueue), threshold(_reportQueue->capacity() / 2), queueOwner(false) {}

ForceableReportQueue::~ForceableReportQueue() {
    if (queueOwner) delete this->reportQueue;
}

bool ForceableReportQueue::forcePush(const report_t &report) const {
    return this->reportQueue->push(report);
}

bool ForceableReportQueue::push(const report_t &report) const {
    return (this->reportQueue->count() < this->threshold) && this->reportQueue->push(report);
}

bool ForceableReportQueue::pop(report_t *report) const {
    return this->reportQueue->pop(report);
}

bool ForceableReportQueue::empty() const {
    return this->reportQueue->empty();
}

bool ForceableReportQueue::full() const {
    return this->reportQueue->count() == this->threshold;
}

uint32_t ForceableReportQueue::count() const {
    if  (this->reportQueue->count() > this->threshold) {
        return this->threshold;
    } else {
        return this->reportQueue->count();
    }
}

uint32_t ForceableReportQueue::capacity() const {
    return this->threshold;
}