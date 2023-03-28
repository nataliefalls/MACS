#include "tusb.h"
#include "bsp/board.h"
#include "usbTask.h"
#include "constants.h"
#include "PicoTimer.h"
#include "ReportQueueHandler.h"

void hidTask(ReportQueueHandler *handler);
bool initialStartUpFinished();
bool pollingIntervalWait();
void requeueReport(report_t &report);

IReportQueue *reportQueue;
ReportQueueHandler *handler;

int usbMain(IReportQueue *queue) {
  board_init();
  tud_init(BOARD_TUD_RHPORT); // init device stack on configured roothub port

  reportQueue = queue; // keep a global reference of the queue
  *handler = new ReportQueueHandler(queue);

  while (1) {
    tud_task();
    hidTask(handler);
  }

  return 0;
}

void hidTask(ReportQueueHandler *handler) {
  if (!initialStartUpFinished()) return;
  if (pollingIntervalWait()) return;
  
  report_t report;
  switch (handler->sendNextReport(report)) {
    case E_QUEUE_EMPTY: return;
    case E_USB_TRANSFER_FAILED: 
      requeueReport(report, handler);
    case SEND_SUCCESS: return;
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