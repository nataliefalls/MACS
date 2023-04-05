#include "usbTask.h"
#include "usbTiming.h"
#include "usbUtil.h"

#include "ReportQueueHandler.h"
#include "ForceableReportQueueController.h"
#include "ForceableReportQueue.h"

int usbMain(IForceableReportQueue *queue) {
  ReportQueueHandler *handler = new ReportQueueHandler(queue);
  IForceableReportQueueController *controller = new ForceableReportQueueController(queue);

  while (true) {
    tud_task();
    hidTask(handler, controller);
  }
  return 0;
}