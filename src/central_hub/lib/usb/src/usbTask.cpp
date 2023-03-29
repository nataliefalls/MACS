#include "tusb.h"
#include "bsp/board.h"
#include "usbTask.h"
#include "constants.h"
#include "PicoTimer.h"
#include "ReportQueueHandler.h"
#include "ForceableReportQueueController.h"

void hidTask(ReportQueueHandler *handler, IForceableReportQueueController *controller);
bool initialStartUpFinished();
bool pollingIntervalWait();
// void requeueReport(report_t &report);

int usbMain(IForceableReportQueue *queue) {
  board_init();
  tud_init(BOARD_TUD_RHPORT); // init device stack on configured roothub port

  ReportQueueHandler *handler = new ReportQueueHandler(queue);
  IForceableReportQueueController *controller = new ForceableReportQueueController(queue);

  while (1) {
    tud_task();
    hidTask(handler, controller);
  }

  return 0;
}

void hidTask(ReportQueueHandler *handler, IForceableReportQueueController *controller) {
  if (!initialStartUpFinished()) return;
  if (pollingIntervalWait()) return;
  
  report_t report;
  handler->sendNextReport(report);
  switch (handler->sendNextReport(report)) {
    case E_USB_TRANSFER_FAILED:
      /**
       * In this case, the report was queued, but for some reason, the transfer failed, so
       * we have to force this report back onto the queue to be processed again later.
       * Otherwise, in the case of an...
       * 
       * - input report: the module won't update until the next state change
       * - connected report: the connected module won't appear on the application
       * - disconnected report: the disconnected module won't disappear from the screen
      */
      controller->forceReport(report);
    default: return;
  }
}

bool initialStartUpFinished() {
  static PicoTimer startUpTimer;
  static bool startTimeInitialized = false;

  if (!startTimeInitialized) {
    startTimeInitialized = true;
    startUpTimer.startTimer(1000000 / 2);
  }

  return startUpTimer.timeoutOccured();
}

bool pollingIntervalWait() {
  static PicoTimer pollingTimer;
  static bool start_us_initialized = false;

  if (!start_us_initialized) {
    pollingTimer.startTimer(POLLING_INTERVAL_MS * 1000);
    start_us_initialized = true;
  }

  if (pollingTimer.timeoutOccured()) {
    pollingTimer.startTimer(POLLING_INTERVAL_MS * 1000);
    return false;
  } else {
    return true;
  }
}