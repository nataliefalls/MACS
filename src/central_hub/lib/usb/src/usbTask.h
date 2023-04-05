#pragma once
#include "IReportQueue.h"

void usbTask(IReportQueue *inputQueue, IReportQueue *connectionQueue);

void usbInit();