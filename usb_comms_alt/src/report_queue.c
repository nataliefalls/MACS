#include "report_queue.h"

// buffer for the report queue
static report_t queue[REPORT_QUEUE_BUFFER_SIZE];
// index at which the next report will be pushed
static uint8_t pushIndex = 0;
// number of reports on the queue waiting to be sent
static uint8_t reportsLeft = 0;

/**
 * is the queue full?
*/
bool queue_full() {
    return reportsLeft == REPORT_QUEUE_BUFFER_SIZE;
}

/**
 * is the queue empty?
*/
bool queue_empty() {
    return reportsLeft == 0;
}

/**
 * the index at which the next report should be popped
 * assumes the queue is non-empty
*/
uint8_t pop_index() {
    if (reportsLeft > pushIndex) {
        uint8_t reportsBeforeIndexZero = reportsLeft - pushIndex;
        return REPORT_QUEUE_BUFFER_SIZE - reportsBeforeIndexZero;
    } else {
        return pushIndex - reportsLeft;
    }
}

/**
 * push a report onto the queue to be sent over usb
 * false return indicates the queue is full, and report could not be pushed
 * true return indicates success
*/
bool queue_push(report_t report) {
    if (queue_full()) return false;

    queue[pushIndex] = report;
    reportsLeft++;

    if (++pushIndex >= REPORT_QUEUE_BUFFER_SIZE) {
        pushIndex -= REPORT_QUEUE_BUFFER_SIZE;
    }

    return true;
}

/**
 * pop a report off of the queue
 * false return indicates the queue is empty, and no report was popped
 * true return indicates a report was popped, in which case, the given report arg will contain the report
*/
bool queue_pop(report_t *report) {
    if (queue_empty()) return false;

    *report = queue[pop_index()];
    reportsLeft--;

    return true;
}

/**
 * the number of reports that can be added to the queue before it's at maximum capacity
*/
uint8_t space_remaining() {
    return REPORT_QUEUE_BUFFER_SIZE - reportsLeft;
}