#ifndef REPORT_HANDLER_H
#define REPORT_HANDLER_H

#include <stdbool.h>

/**
 * send the next report in the report queue over usb
*/
bool send_next_report();

#endif