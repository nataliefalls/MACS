#pragma once
#include "IForceableReportQueue.h"

/**
 * main function for USB thread.
 * 
 * takes a shared report queue as an argument. it's important that
 * the queue is "forceable" so that we can push reports back onto
 * the report queue (if necessary) without fear of the queue being
 * completely full, which may otherwise stall USB report handling.
*/
int usbMain(IForceableReportQueue *queue);