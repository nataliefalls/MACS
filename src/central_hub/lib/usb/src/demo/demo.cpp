#include "bsp/board.h"
#include "hardware/adc.h"

#include "demo.h"
#include "report_types.h"
#include "ReportQueueController.h"
#include "ReportQueueHandler.h"
#include "PicoQueueReportQueue.h"

#include "Module.h"
#include "ButtonPayload.h"
#include "JoystickPayload.h"

#define TOGGLE_CONNECTION_PIN 13
#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

Module *buttonModule;
Module *joystickModule;
const uint8_t buttonModuleID = 0x68;
const uint8_t joystickModuleID = 0x69;

void connectButtonModule(ReportQueueController *controller) {
  buttonModule = new Module(buttonModuleID, { 0, 1 }, controller);
}

void connectJoystickModule(ReportQueueController *controller) {
  joystickModule = new Module(joystickModuleID, { 1, 1 }, controller);
}

bool updateButtonModule() {
  ButtonPayload *buttonPayload = new ButtonPayload({ read_button(BUTTON_PIN) });
  bool updated = buttonModule->update(buttonPayload);
  if (!updated) {
    delete buttonPayload;
    return false;
  } else {
    return true;
  }
}

bool updateJoystickModule() {
  JoystickPayload *joystickPayload = new JoystickPayload({
    adc_read_pin(JOYSTICK_X_PIN),
    adc_read_pin(JOYSTICK_Y_PIN),
    0
  });
  if (!joystickModule->update(joystickPayload)) {
    delete joystickPayload;
    return false;
  } else {
    return true;
  }
}

void removeButtonModule() {
  delete buttonModule;
}

void removeJoystickModule() {
  delete joystickModule;
}

bool toggleButtonConnection(bool &connected, ReportQueueController *controller) {
  static uint8_t buttonPreviouslyHeld = false;
  bool ret = false;
  uint8_t toggleConnection = read_button(TOGGLE_CONNECTION_PIN);

  // only toggle on rising edge
  if (toggleConnection && !buttonPreviouslyHeld) {
    if (connected) {
      removeButtonModule();
    } else {
      connectButtonModule(controller);
    }
    connected = !connected;
    ret = true;
  }

  buttonPreviouslyHeld = toggleConnection;
  return ret;
}

bool toggleJoystickConnection(bool &connected, ReportQueueController *controller) {
  static uint8_t joystickPreviouslyHeld = false;
  bool ret = false;
  uint8_t toggleConnection = read_button(TOGGLE_CONNECTION_PIN);

  // only toggle on rising edge
  if (toggleConnection && !joystickPreviouslyHeld) {
    if (connected) {
      removeJoystickModule();
    } else {
      connectJoystickModule(controller);
    }
    connected = !connected;
    ret = true;
  }

  joystickPreviouslyHeld = toggleConnection;
  return ret;
}

bool queueButtonReport(ReportQueueController *controller) {
  static bool buttonConnected = false;

  if (!toggleButtonConnection(buttonConnected, controller) && buttonConnected) {
    return updateButtonModule();
  }

  return false;
}

bool queueJoystickReport(ReportQueueController *controller) {
  static bool joystickConnected = false;

  if (!toggleJoystickConnection(joystickConnected, controller) && joystickConnected) {
    return updateJoystickModule();
  }

  return false;
}

void queueDemoReport(ReportQueueController *controller) {
  static bool buttonOrJoystick = false;

  if (buttonOrJoystick) {
    queueButtonReport(controller);
  } else {
    queueJoystickReport(controller);
  }

  buttonOrJoystick = !buttonOrJoystick;
}

void init_adc_gpio() {
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_down(BUTTON_PIN);
  
  gpio_init(TOGGLE_CONNECTION_PIN);
  gpio_set_dir(TOGGLE_CONNECTION_PIN, GPIO_IN);
  gpio_pull_down(TOGGLE_CONNECTION_PIN);

  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
}

inline uint8_t adc_read_pin(uint32_t pin) {
  adc_select_input(pin);
  return (adc_read() >> 4);
}

inline uint8_t read_button(uint32_t pin) {
  return gpio_get(pin) ? 1 : 0;
}