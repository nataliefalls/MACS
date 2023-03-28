#include "tusb.h"
#include "bsp/board.h"
#include "usbTask.h"
#include "constants.h"
#include "PicoTimer.h"

#ifdef DEMO_FOR_FUENTES
  #include "demo.h"
  #include "demoUtils.h"
#endif

void hidTask();
bool initialStartUpFinished();
bool pollingIntervalWait();

#ifdef DEMO_FOR_FUENTES
int main() {
  usbMain();
}
#endif

int usbMain() {
  board_init();
  tud_init(BOARD_TUD_RHPORT); // init device stack on configured roothub port

  #ifdef DEMO_FOR_FUENTES
    init_adc_gpio();
  #endif

  while (1) {
    tud_task();
    hidTask();
  }

  return 0;
}

void hidTask() {
  if (!initialStartUpFinished()) return;
  if (pollingIntervalWait()) return;
  
  #ifdef DEMO_FOR_FUENTES
    send_demo_report();
  #else
    handler->sendNextReport();
  #endif
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