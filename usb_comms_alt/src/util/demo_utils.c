#include "bsp/board.h"
#include "hardware/adc.h"

#include "demo_utils.h"
#include "report_types.h"
#include "usb_controller.h"
#include "report_queue_handler.h"

#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

void init_adc_gpio(void) {
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_down(BUTTON_PIN);

  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
}

void send_button_report_demo(void) {
  if ( !tud_hid_ready() ) return;

  uint8_t moduleID = 0x68;
  button_data_t report = {
    .button = gpio_get(BUTTON_PIN) ? 1 : 0,
  };

  // tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
  button_report(moduleID, report);
  switch (send_next_report()) {
    case 0: break;
    case E_QUEUE_EMPTY: {
      button_report_t report = {
        .moduleID = E_QUEUE_EMPTY,
        .button = 1,
      };
      tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
      break;
    }
    case E_USB_TRANSFER_FAILED: {
      button_report_t report = {
        .moduleID = E_USB_TRANSFER_FAILED,
        .button = 0,
      };
      tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
      break;
    }
  }
}

void send_joystick_report_demo(void) {
  if ( !tud_hid_ready() ) return;
  adc_select_input(JOYSTICK_X_PIN);
  uint16_t x = adc_read();
  adc_select_input(JOYSTICK_Y_PIN);
  uint16_t y = adc_read();

  uint8_t moduleID = 0x68;
  joystick_data_t report = {
    .x = x,
    .y = y,
  };

  // tud_hid_report(REPORT_ID_JOYSTICK_DATA, &report, sizeof(report));
  joystick_report(moduleID, report);
  switch (send_next_report()) {
    case 0: break;
    case E_QUEUE_EMPTY: {
      button_report_t report = {
        .moduleID = E_QUEUE_EMPTY,
        .button = 1,
      };
      tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
      break;
    }
    case E_USB_TRANSFER_FAILED: {
      button_report_t report = {
        .moduleID = E_USB_TRANSFER_FAILED,
        .button = 0,
      };
      tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
      break;
    }
  }
}

void send_demo_report(void) {
  static bool buttonOrJoystick = false;

  if (buttonOrJoystick) {
    send_joystick_report_demo();
  } else {
    send_button_report_demo();
  }

  buttonOrJoystick = !buttonOrJoystick;
}