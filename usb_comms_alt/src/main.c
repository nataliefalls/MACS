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

#define DEMO_FOR_FUENTES // uncomment to build a demo version
#define USE_TIMESTAMPS // uncomment to use timestamps for our wait

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tusb_config.h"
#include "tusb.h"
#include "class/hid/hid.h"
#include "bsp/board.h"

#include "usb_descriptors.h"
#include "report_types.h"
#include "pico/stdlib.h"

#ifdef DEMO_FOR_FUENTES
#include "hardware/adc.h"

#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1
#endif // DEMO_FOR_FUENTES

void hid_task(void);

/*------------- MAIN -------------*/
int main(void) {
  board_init();

  #ifdef DEMO_FOR_FUENTES
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_down(BUTTON_PIN);

  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
  #endif // DEMO_FOR_FUENTES

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  while (1) {
    tud_task(); // tinyusb device task
    hid_task();
  }

  return 0;
}

//--------------------------------------------------------------------+
// Demo utilities
//--------------------------------------------------------------------+
#ifdef DEMO_FOR_FUENTES

void send_button_report_demo(void) {
  if ( !tud_hid_ready() ) return;

  button_report_t report = {
    .moduleID = 0x68,
    .button = gpio_get(BUTTON_PIN) ? 1 : 0,
  };

  tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
}

void send_joystick_report_demo(void) {
  if ( !tud_hid_ready() ) return;
  adc_select_input(JOYSTICK_X_PIN);
  uint16_t x = adc_read();
  adc_select_input(JOYSTICK_Y_PIN);
  uint16_t y = adc_read();

  joystick_report_t report = {
    .moduleID = 0x68,
    .joystick.x = x,
    .joystick.y = y,
  };

  tud_hid_report(REPORT_ID_JOYSTICK_DATA, &report, sizeof(report));
}

static void send_demo_report(void) {
  static bool buttonOrJoystick = false;

  if (buttonOrJoystick) {
    send_joystick_report_demo();
  } else {
    send_button_report_demo();
  }

  buttonOrJoystick = !buttonOrJoystick;
}

#endif // DEMO_FOR_FUENTES

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
  // nothing yet
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
  // nothing yet
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
    // nothing yet
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

bool initialStartUpFinished(void) {
  static bool startTimeInitialized = false;
  static uint32_t startTime;

  if (!startTimeInitialized) {
    startTime = time_us_32();
    startTimeInitialized = true;
  }

  if (time_us_32() - startTime < 1000000 / 2) {
    return false;
  }
  return true;
}

bool polling_interval_wait() {
  #ifdef USE_TIMESTAMPS
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
  #else
  static uint32_t start_ms = 0;
  if ( board_millis() - start_ms < POLLING_INTERVAL_MS) { // not enough time
    return true;
  } else {
    start_ms += POLLING_INTERVAL_MS;
    return false;
  }
  #endif

}

void send_hid_report(void) {
}

void hid_task(void) {
  if (!initialStartUpFinished()) {
    return;
  } else if (polling_interval_wait()) {
    return;
  }
  
  #ifdef DEMO_FOR_FUENTES
  send_demo_report();
  #else
  send_next_report();
  #endif
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len) {
  (void) instance;
  (void) len;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  // by default, just zero out the buffer
  memset(buffer, 0, reqlen);
  
  // For now, we don't have any reports to send on initialization
  // it may be helpful for the host to request all connected modules
  // on start-up, but that seems complicated, so I'll do that later.
  return reqlen;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) instance;
}