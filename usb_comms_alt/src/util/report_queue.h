#ifndef REPORT_QUEUE_H
#define REPORT_QUEUE_H

#include "report_types.h"

#define REPORT_QUEUE_BUFFER_SIZE 100

/**
 * push a report onto the queue to be sent over usb
 * false return indicates the buffer is full, and report could not be pushed
 * true return indicates success
*/
bool queue_push(report_t report);

/**
 * pop a report off of the queue, returns the report
*/
report_t queue_pop();

/**
 * returns the number of reports to be sent in the buffer
*/
uint8_t reports_left();

#endif