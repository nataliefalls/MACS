#include "tusb_config.h"
#include "bsp/board.h"

#include "usbUtil.h"
#include "usbTiming.h"

void hidTask(ReportQueueHandler *handler, IForceableReportQueueController *controller) {
  report_t report;
  if (handler->sendNextReport(report) == E_USB_TRANSFER_FAILED) {
      /**
       * In this case, the report was queued, but for some reason, the USB transfer failed.
       * We have to make sure this report makes it back onto the queue to be processed
       * again later. Otherwise, in the case of an...
       * 
       * - input report: the module input won't update until the next state change
       * - connected report: the connected module won't appear in the application
       * - disconnected report: the disconnected module won't disappear from the application
      */
      controller->forceReport(report);
  }
}

void usbInit() {
  static bool init = false;
  if (init) return;

  tud_init(BOARD_TUD_RHPORT); // init device stack on configured roothub port

  do { tud_task(); } while (!initialStartUpFinished());

  init = true;
}