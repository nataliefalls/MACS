#ifndef REPORT_HANDLER_H
#define REPORT_HANDLER_H

#include <stdint.h>
#include "tusb.h"
#include "report_types.h"

/**
 * send the next report in the report queue over usb
*/
bool send_report(report_t report);

#endif