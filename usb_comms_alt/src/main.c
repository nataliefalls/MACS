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

/*------------- MAIN -------------*/
int main(void)
{
  board_init();

  // init device stack on configured roothub port
  // tud_init(BOARD_TUD_RHPORT);
  tusb_init();

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
static int send_hid_report(uint8_t report_id)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return 2;

  switch(report_id)
  {
    case REPORT_ID_MODULE_CONNECTED:
    {
      // send next module connected message
      static bool sent = false;
      if (!sent) {
        module_connected_report_t report = {
          .module_id = 1,
          .coordinates = { 0, 0, 0 }
        };
        tud_hid_report(report_id, &report, sizeof(module_connected_report_t));
        sent = true;
        return 0;
      } else {
        return 1;
      }
    }
    break;
    case REPORT_ID_MODULE_DISCONNECTED:
    {
      return 1;
      // send next module disconnected message
    }
    break;
    case REPORT_ID_BUTTON_DATA:
    {
      // send next button data message
      button_report_t report = {
        .module_id = 2,
        .button = 0
      };
      tud_hid_report(report_id, &report, sizeof(button_report_t));
      return 0;
    }
    break;
    case REPORT_ID_DPAD_DATA:
    {
      return 1;
      // send next dpad data message
    }
    break;
    case REPORT_ID_JOYSTICK_DATA:
    {
      return 1;
      // send next joystick data message
    }
    break;
    default: 
    {
      return 1;
    }
    break;
  }
}

void send_next_report(report_id_t start) {
  int sent;
  report_id_t id = start;
  
  // we're not guaranteed to have a report for each report_id, so send the next one
  // immediately if this report was not sent
  do {
    sent = send_hid_report(id);
    if (sent == 1) { // hid is ready, but no report was sent
      id = id + 1;
    }
  } while (sent == 1 && id < REPORT_ID_COUNT); // stop once we reach the last report_id
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 500;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  // test
  // button_report_t report = {
  //   .module_id = 2,
  //   .button = 0
  // };
  module_connected_report_t report = {
    .module_id = 1,
    .coordinates = { 0, 0, 0 }
  };
  tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(button_report_t));
  

  // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
  // send_next_report(REPORT_ID_MODULE_CONNECTED);
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