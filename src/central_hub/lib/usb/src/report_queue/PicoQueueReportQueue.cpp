#include "pico/multicore.h"
#include "PicoTimer.h"
#include "PicoQueueReportQueue.h"

PicoQueueReportQueue::PicoQueueReportQueue(queue_t *_queue): queue(_queue) {
    queue_init(this->queue, sizeof(report_t), 8);
}

PicoQueueReportQueue::~PicoQueueReportQueue() {
    queue_free(this->queue);
}

bool PicoQueueReportQueue::queuePush(const report_t &report) const {
    PicoTimer timer;
    timer.startTimer(this->QUEUE_TIMEOUT);

    do {
        if (queue_try_add(this->queue, &report)) return true;
    } while (!timer.timeoutOccured());

    return false;
}

bool PicoQueueReportQueue::queuePop(report_t *report) const {
    PicoTimer timer;
    timer.startTimer(this->QUEUE_TIMEOUT);

    do {
        if (queue_try_remove(this->queue, report)) return true;
    } while (!timer.timeoutOccured());
    
    return false;
}

bool PicoQueueReportQueue::queuePeek(report_t *report) const {
    PicoTimer timer;
    timer.startTimer(this->QUEUE_TIMEOUT);

    do {
        if (queue_try_peek(this->queue, report)) return true;
    } while (!timer.timeoutOccured());
    
    return false;
}