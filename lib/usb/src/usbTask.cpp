#include "usbTask.h"
#include "constants.h"
#include "PicoTimer.h"
#include "tusb.h"

bool isInitialStartUpFinished();
bool pollingIntervalTimeout();

void usbTask(IReportQueueHandler *handler) {
  while (1) {
    // this is a tiny USB thing. if it's not called every iteration, the USB port stops working
    tud_task();
    // process any reports we see off of the shared queue
    if (pollingIntervalTimeout()) {
      handler->sendNextReport();
    }
  }
}

void usbInit() {
  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);
  // let USB enumeration finish before sending any reports
  do {
    tud_task();
  } while (!isInitialStartUpFinished());
}

/*******************************************************
 * TIMING HELPER FUNCTIONS
********************************************************/

bool isInitialStartUpFinished() {
  static PicoTimer startUpTimer;
  static bool startTimeInitialized = false;

  if (!startTimeInitialized) {
    startTimeInitialized = true;
    startUpTimer.startTimer(1000000 / 2);
  }

  return startUpTimer.timeoutOccured();
}

bool pollingIntervalTimeout() {
  static PicoTimer pollingTimer;
  static bool start_us_initialized = false;

  if (!start_us_initialized) {
    pollingTimer.startTimer(POLLING_INTERVAL_MS * 1000);
    start_us_initialized = true;
  }

  if (pollingTimer.timeoutOccured()) {
    pollingTimer.startTimer(POLLING_INTERVAL_MS * 1000);
    return true;
  } else {
    return false;
  }
}