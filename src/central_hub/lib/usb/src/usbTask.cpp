#include "tusb.h"
#include "usbTask.h"
#include "constants.h"
#include "PicoTimer.h"
#include "ReportQueueHandler.h"

void hidTask(ReportQueueHandler *handler);
bool initialStartUpFinished();
bool pollingIntervalWait();

void usbInit() {
  do {
    tud_task();
  } while (!initialStartUpFinished());
}

void usbMain(IReportQueue *queue) {
  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);
  // wait for usb enumeration to finish before sending reports
  usbInit();

  ReportQueueHandler *handler = new ReportQueueHandler(queue);

  while (1) {
    tud_task();
    hidTask(handler);
  }
}

void hidTask(ReportQueueHandler *handler) {
  if (pollingIntervalWait()) {
    handler->sendNextReport();
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