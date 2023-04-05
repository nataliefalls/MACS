#include "pico/multicore.h"
#include "MulticoreFifoReportQueue.h"

bool MulticoreReportQueue::push(const report_t &report) const {
    return multicore_fifo_push_timeout_us(*((uint32_t*)(&report)), this->QUEUE_TIMEOUT);
}

bool MulticoreReportQueue::pop(report_t *report) const {
    return multicore_fifo_pop_timeout_us(this->QUEUE_TIMEOUT, (uint32_t *)(report));
}