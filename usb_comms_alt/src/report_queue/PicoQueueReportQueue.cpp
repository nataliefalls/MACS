#include "pico/util/queue.h"
#include "PicoQueueReportQueue.h"

PicoQueueReportQueue::PicoQueueReportQueue() {
    queue_init(this->queue, sizeof(report_t), this->QUEUE_SIZE);
}

bool PicoQueueReportQueue::queue_push(const report_t &report) const {
    // stuff
    return queue_try_add(this->queue, &report);
}

bool PicoQueueReportQueue::queue_pop(report_t *report) const {
    // stuff
    return queue_try_remove(this->queue, report);
}