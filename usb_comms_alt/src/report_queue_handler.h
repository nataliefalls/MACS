#ifndef REPORT_HANDLER_H
#define REPORT_HANDLER_H

#include <stdbool.h>

// error status codes for send_next_report
enum {
    E_QUEUE_EMPTY = 1,
    E_USB_TRANSFER_FAILED,
};

/**
 * send the next report in the report queue over usb
*/
int send_next_report();

#endif