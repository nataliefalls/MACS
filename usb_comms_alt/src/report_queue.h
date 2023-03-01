#ifndef REPORT_QUEUE_H
#define REPORT_QUEUE_H

#include "report_types.h"

#define REPORT_QUEUE_BUFFER_SIZE 100

/**
 * push a report onto the queue to be sent over usb
 * false return indicates the queue is full, and report could not be pushed
 * true return indicates success
*/
bool queue_push(report_t report);

/**
 * pop a report off of the queue
 * false return indicates the queue is empty, and no report was popped
 * true return indicates a report was popped, in which case, the given report arg will contain the report
*/
bool queue_pop(report_t *report);

/**
 * the number of reports that can be added to the queue before it's at maximum capacity
*/
uint8_t space_remaining();

#endif