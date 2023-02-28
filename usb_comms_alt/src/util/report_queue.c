#include "report_queue.h"

static report_t queue[REPORT_QUEUE_BUFFER_SIZE];
static uint8_t pushIndex = 0;
static uint8_t popIndex = 0;

/**
 * push a report onto the queue to be sent over usb
 * false return indicates the buffer is full, and report could not be pushed
 * true return indicates success
*/
bool queue_push(report_t report) {
    if (pushIndex == popIndex) return false;

    queue[pushIndex].id = report.id;
    queue[pushIndex].payload = report.payload;

    if (++pushIndex >= REPORT_QUEUE_BUFFER_SIZE) {
        pushIndex -= REPORT_QUEUE_BUFFER_SIZE;
    }

    return true;
}

/**
 * pop a report off of the queue, returns the report
*/
report_t queue_pop() {
    report_t report = queue[popIndex];

    if (++popIndex >= REPORT_QUEUE_BUFFER_SIZE) {
        popIndex -= REPORT_QUEUE_BUFFER_SIZE;
    }

    return report;
}

/**
 * returns the number of reports to be sent in the buffer
*/
uint8_t reports_left() {
    return (pushIndex < popIndex) ?
        REPORT_QUEUE_BUFFER_SIZE - popIndex + pushIndex :
        pushIndex - popIndex;
}