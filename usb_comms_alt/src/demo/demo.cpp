#include "bsp/board.h"
#include "hardware/adc.h"

#include "demo.h"
#include "report_types.h"
#include "ReportQueueController.h"
#include "ReportQueueHandler.h"
#include "MulticoreReportQueue.h"

#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

IReportQueue *queue = new MulticoreReportQueue();
ReportQueueHandler *handler = new ReportQueueHandler(queue);
ReportQueueController *controller = new ReportQueueController(queue);

void init_adc_gpio() {
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_down(BUTTON_PIN);

  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
}

void send_button_report_demo() {
  if ( !tud_hid_ready() ) return;

  uint8_t moduleID = 0x68;
  uint8_t button = gpio_get(BUTTON_PIN) ? 1 : 0;

  payload_t payload = {
    .button = { button },
  };

  // tud_hid_report(REPORT_ID_BUTTON_DATA, &report, sizeof(report));
  // button_report(moduleID, report);
  controller->inputReport(moduleID, REPORT_ID_BUTTON_DATA, payload);
  switch (handler->sendNextReport()) {
    case SEND_SUCCESS:
      break;
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

static inline uint16_t adc_read_pin(uint pin) {
  adc_select_input(pin);
  return adc_read();
}

void send_joystick_report_demo() {
  if ( !tud_hid_ready() ) return;
  uint16_t x = adc_read_pin(JOYSTICK_X_PIN);
  uint16_t y = adc_read_pin(JOYSTICK_Y_PIN);
  uint8_t moduleID = 0x68;

  const payload_t payload = {
    .joystick = {
      .x = (uint8_t) (x >> 8),
      .y = (uint8_t) (y >> 8),
    },
  };

  // tud_hid_report(REPORT_ID_JOYSTICK_DATA, &report, sizeof(report));
  controller->inputReport(moduleID, REPORT_ID_JOYSTICK_DATA, payload);
  switch (handler->sendNextReport()) {
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

void send_demo_report() {
  static bool buttonOrJoystick = false;

  if (buttonOrJoystick) {
    send_joystick_report_demo();
  } else {
    send_button_report_demo();
  }

  buttonOrJoystick = !buttonOrJoystick;
}