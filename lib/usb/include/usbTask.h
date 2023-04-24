#pragma once

#include "IReportQueueHandler.h"

/**
 * main entry point for the USB thread. Responsibilities:
 * - handle reports off of the reports off of the input and connection queue
 * - facilitate USB communication
 * 
 * @param handler a report queue handler, with which we process reports
*/
void usbTask(IReportQueueHandler *handler);

/**
 * initialize the tinyUSB stack and identify as a human interface device
 * this function blocks until USB enumeration is complete, usually within 50ms
*/
void usbInit();