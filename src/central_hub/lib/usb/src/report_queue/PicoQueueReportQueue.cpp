#include "PicoTimer.h"
#include "PicoQueueReportQueue.h"

PicoQueueReportQueue::PicoQueueReportQueue(queue_t *_queue, uint32_t capacity): queue(_queue) {
    queue_init(this->queue, sizeof(report_t), capacity);
}

PicoQueueReportQueue::PicoQueueReportQueue(queue_t *_queue):
    PicoQueueReportQueue(_queue, 4) {}

PicoQueueReportQueue::~PicoQueueReportQueue() {
    queue_free(this->queue);
}

bool PicoQueueReportQueue::push(const report_t &report) const {
    PicoTimer timer;
    timer.startTimer(this->QUEUE_TIMEOUT);

    do {
        if (queue_try_add(this->queue, &report)) return true;
    } while (!timer.timeoutOccured());

    return false;
}

bool PicoQueueReportQueue::pop(report_t *report) const {
    PicoTimer timer;
    timer.startTimer(this->QUEUE_TIMEOUT);

    do {
        if (queue_try_remove(this->queue, report)) return true;
    } while (!timer.timeoutOccured());
    
    return false;
}

bool PicoQueueReportQueue::empty() const {
    return queue_is_empty(this->queue);
}

bool PicoQueueReportQueue::full() const {
    return queue_is_full(this->queue);
}

uint32_t PicoQueueReportQueue::count() const {
    return queue_get_level(this->queue);
}

uint32_t PicoQueueReportQueue::capacity() const {
    return this->queue->element_count;
}