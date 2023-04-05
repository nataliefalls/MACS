#include "usbTask.h"
#include "constants.h"
#include "PicoTimer.h"
#include "ReportQueueHandler.h"

void hidTask(ReportQueueHandler *handler);
bool initialStartUpFinished();
bool pollingIntervalWait();

/**
 * initialize the tinyUSB stack and identify as a human interface device
 * this function blocks until USB enumeration is complete, usually within 50ms
*/
void usbInit() {
  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);
  // let USB enumeration finish before sending any reports
  do {
    tud_task();
  } while (!initialStartUpFinished());
}

/**
 * main entry point for the USB thread. Responsibilities:
 * - handle reports off of the input queue
 * - facilitate USB communication
 * 
 * @param inputQueue the shared queue for controller input messages
 * @param connectionQueue the shared queue for module (dis)connection messages
*/
void usbTask(IReportQueue *inputQueue, IReportQueue *connectionQueue) {

  ReportQueueHandler *handler = new ReportQueueHandler(inputQueue, connectionQueue);

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