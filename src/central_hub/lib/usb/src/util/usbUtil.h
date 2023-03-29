#pragma once

#include "ReportQueueHandler.h"
#include "IForceableReportQueueController.h"

/**
 * handle reports from the report queue
 * 
 * in the case that a report we pop off of the queue can't be sent over USB,
 * we force the report back onto the queue
*/
void hidTask(ReportQueueHandler *handler, IForceableReportQueueController *controller);

/**
 * initialize USB capabilities
 * 
 * blocks until all USB descriptors are transmitted
 * subsequent calls to this function do nothing and return immediately
*/
void usbInit();