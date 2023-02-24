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

#include "tusb_config.h"
#include "tusb.h"
#include "class/hid/hid.h"
#include "bsp/board.h"

#include "usb_descriptors.h"
#include "reports.h"
#include "pico/stdlib.h"

#define BUTTON_PIN 14

void hid_task(void);

/*------------- MAIN -------------*/
int main(void)
{
  board_init();
  // gpio_init(BUTTON_PIN);
  // gpio_set_dir(BUTTON_PIN, GPIO_IN);
  // gpio_pull_down(BUTTON_PIN);

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);
  // tusb_init();

  while (1)
  {
    tud_task(); // tinyusb device task
    hid_task();
  }

  return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  // nothing yet
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  // nothing yet
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    // nothing yet
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// return type indicates status
// 0 - report was sent
// 1 - hid is ready && no report was sent for given id
// 2 - hid is not ready
static void send_hid_report(void)
{
  if ( !tud_hid_ready() ) return;

  button_report_t report = {
    .module_id = 0x68,
    .button = 1,
  };
  tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // static bool startTimeInitialized = false;
  // static uint32_t startTime;
  // if (!startTimeInitialized) {
  //   startTime = time_us_32();
  //   startTimeInitialized = true;
  // }
  // if (time_us_32() - startTime < 1000000 / 2) {
  //   return;
  // }

  // Poll every 10ms
  // static bool start_us_initialized = false;
  // static uint32_t start_us;
  // if (!start_us_initialized) {
  //   start_us = time_us_32();
  //   start_us_initialized = true;
  // }
  // if (time_us_32() - start_us < 10000) {
  //   return;
  // }
  // start_us = time_us_32();

  // static uint32_t interval_ms = 500;
  // static uint32_t start_ms = 0;

  // if ( board_millis() - start_ms < interval_ms) return; // not enough time
  // start_ms += interval_ms;

  send_hid_report();

  // static bool pressed;
  // // pressed = gpio_get(BUTTON_PIN);
  // pressed = !pressed;

  // if (pressed) {
  //   report.module_id = 105;
  //   report.button = 1;
  //   tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(button_report_t));
  // } else {
  //   report.module_id = 105;
  //   report.button = 0;
  //   tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(button_report_t));
  // }


  // static module_disconnected_report_t disconnectionReport = {
  //   .module_id = 2,
  // };
  // static module_connected_report_t connectionReport = {
  //   .module_id = 1,
  //   .coordinates = { 2, 3, 4 }
  // };
  // static bool connection = true;
  // if (connection) {
  //   tud_hid_report(REPORT_ID_MODULE_CONNECTED, &connectionReport, sizeof(module_connected_report_t));
  // } else {
  //   tud_hid_report(REPORT_ID_MODULE_DISCONNECTED, &disconnectionReport, sizeof(module_disconnected_report_t));
  // }
  // connection = !connection;
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
  (void) instance;
  (void) len;

  // uint8_t next_report_id = (report[0] + 1) % REPORT_ID_COUNT;
  
  // send_next_report(next_report_id);
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // by default, just zero out the buffer
  memset(buffer, 0, reqlen);
  
  // For now, we don't have any reports to send on initialization
  // it may be helpful for the host to request all connected modules
  // on start-up, but that seems complicated, so I'll do that later.
  return reqlen;

  switch(report_id) {
    case REPORT_ID_MODULE_CONNECTED:
      break;
    case REPORT_ID_MODULE_DISCONNECTED:
      break;
    case REPORT_ID_BUTTON_DATA:
      break;
    case REPORT_ID_DPAD_DATA:
      break;
    case REPORT_ID_JOYSTICK_DATA:
      break;
    default:
      break;
  }
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;
}