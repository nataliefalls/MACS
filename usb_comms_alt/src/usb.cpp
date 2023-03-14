/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// tinyusb includes
#include "tusb_config.h"
#include "tusb.h"

// pico includes
#include "bsp/board.h"
#include "pico/time.h"

// local includes
#include "usb.h"
#include "usb_descriptors.h"
#include "report_types.h"
#include "ReportQueueHandler.h"

#define MULTICORE_REPORT_QUEUE

#ifdef MULTICORE_REPORT_QUEUE
  #include "MulticoreReportQueue.h"
  IReportQueue *queue = new MulticoreReportQueue();
#else
  #include "BufferedReportQueue.h"
  IReportQueue *queue = new BufferedReportQueue();
#endif

#ifdef DEMO_FOR_FUENTES
  #include "demo.h"
#endif

void hidTask();
bool initialStartUpFinished();
bool pollingIntervalWait();

ReportQueueHandler *handler = new ReportQueueHandler(queue);

int usbMain() {
  board_init();

  #ifdef DEMO_FOR_FUENTES
    init_adc_gpio();
  #endif

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  while (1) {
    tud_task();
    hidTask();
  }

  return 0;
}

void hidTask() {
  if (!initialStartUpFinished()) {
    return;
  } else if (pollingIntervalWait()) {
    return;
  }
  
  #ifdef DEMO_FOR_FUENTES
    send_demo_report();
  #else
    handler->sendNextReport();
  #endif
}

bool initialStartUpFinished() {
  static bool startTimeInitialized = false;
  static uint32_t startTime;

  if (!startTimeInitialized) {
    startTime = time_us_32();
    startTimeInitialized = true;
  }

  return (time_us_32() - startTime) >= (1000000 / 2);
}

bool pollingIntervalWait() {
  static bool start_us_initialized = false;
  static uint32_t start_us;
  uint32_t current_us = time_us_32();

  if (!start_us_initialized) {
    start_us = current_us;
    start_us_initialized = true;
  }

  if (current_us < start_us) {
    return (current_us + ~start_us + 1) < (POLLING_INTERVAL_MS * 1000);
  } else if (current_us - start_us < POLLING_INTERVAL_MS * 1000) {
    return true;
  } else {
    start_us = current_us;
    return false;
  }
}