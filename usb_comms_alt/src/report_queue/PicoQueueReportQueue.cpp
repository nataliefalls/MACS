#include "pico/multicore.h"
#include "PicoTimer.h"
#include "PicoQueueReportQueue.h"

PicoQueueReportQueue::PicoQueueReportQueue(queue_t *_queue): queue(_queue) {
    queue_init(this->queue, sizeof(report_t), 2);
}

PicoQueueReportQueue::~PicoQueueReportQueue() {
    queue_free(this->queue);
}

bool PicoQueueReportQueue::queue_push(const report_t &report) const {
    PicoTimer timer;
    timer.startTimer(this->QUEUE_TIMEOUT);

    do {
        if (queue_try_add(this->queue, &report)) return true;
    } while (!timer.timeoutOccured());

    return false;
}

bool PicoQueueReportQueue::queue_pop(report_t *report) const {
    PicoTimer timer;
    timer.startTimer(this->QUEUE_TIMEOUT);

    do {
        if (queue_try_remove(this->queue, report)) return true;
    } while (!timer.timeoutOccured());
    
    return false;
}