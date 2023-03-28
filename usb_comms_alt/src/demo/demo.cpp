#include "demo.h"
#include "ReportQueueController.h"
#include "ReportQueueHandler.h"
#include "PicoQueueReportQueue.h"

#include "ModuleUpdateHandler.h"
#include "ButtonPayload.h"
#include "JoystickPayload.h"
#include "demoUtils.h"

#define buttonModuleID 0x68
#define joystickModuleID 0x69

queue_t sharedQueue;
IReportQueue *queue = new PicoQueueReportQueue(&sharedQueue);
ReportQueueHandler *handler = new ReportQueueHandler(queue);
ReportQueueController *controller = new ReportQueueController(queue);

ModuleUpdateHandler<ButtonPayload> *buttonModule;
ModuleUpdateHandler<JoystickPayload> *joystickModule;

bool toggleJoystickConnection(bool &connected);
bool toggleButtonConnection(bool &connected);
void connectButtonModule();
void connectJoystickModule();
bool updateButtonModule();
bool updateJoystickModule();

void send_button_report() {
  static bool buttonConnected = false;

  if (toggleButtonConnection(buttonConnected)) {
    return;
  } else if (buttonConnected && updateButtonModule()) {
    handler->sendNextReport();
  }
}

void send_joystick_report() {
  static bool joystickConnected = false;

  if (toggleJoystickConnection(joystickConnected)) {
    return;
  } else if (joystickConnected && updateJoystickModule()) {
    handler->sendNextReport();
  }
}

void send_demo_report() {
  static bool joystickOrButton = true;

  if (joystickOrButton) {
    send_joystick_report();
  } else {
    send_button_report();
  }

  joystickOrButton = !joystickOrButton;
}

/********************
 * HELPER FUNCTIONS
********************/

void connectButtonModule() {
  buttonModule = new ModuleUpdateHandler<ButtonPayload>(buttonModuleID, { 0, 1 }, controller);
}

void connectJoystickModule() {
  joystickModule = new ModuleUpdateHandler<JoystickPayload>(joystickModuleID, { 1, 1 }, controller);
}

bool updateButtonModule() {
  ButtonPayload *button = new ButtonPayload({ read_button(BUTTON_PIN) });
  if (!buttonModule->update(button)) {
    delete button;
    return false;
  }
  return true;
}

bool updateJoystickModule() {
  JoystickPayload *joystick = new JoystickPayload({
    adc_read_pin(JOYSTICK_X_PIN),
    adc_read_pin(JOYSTICK_Y_PIN),
    read_button(JOYSTICK_BUTTON_PIN),
  });
  if (!joystickModule->update(joystick)) {
    delete joystick;
    return false;
  }
  return true;
}

void removeButtonModule() {
  delete buttonModule;
}

void removeJoystickModule() {
  delete joystickModule;
}

bool toggleButtonConnection(bool &connected) {
  static bool currentlyPressed = false;
  static bool previouslyPressed = false;
  bool ret = false;

  currentlyPressed = read_button(MODULE_CONNECT_PIN) != 0;

  // triggers only on positive edge of button
  // this way, it won't toggle connection if we hold down the button
  if (currentlyPressed && !previouslyPressed) {
    connected ? removeButtonModule() : connectButtonModule();
    connected = !connected;
    handler->sendNextReport();
    ret = true;
  }
  previouslyPressed = currentlyPressed;
  return ret;
}

bool toggleJoystickConnection(bool &connected) {
  static bool currentlyPressed = false;
  static bool previouslyPressed = false;
  bool ret = false;

  currentlyPressed = read_button(MODULE_CONNECT_PIN) != 0;

  // triggers only on positive edge of button
  // this way, it won't toggle connection if we hold down the button
  if (currentlyPressed && !previouslyPressed) {
    connected ? removeJoystickModule() : connectJoystickModule();
    connected = !connected;
    handler->sendNextReport();
    ret = true;
  }
  previouslyPressed = currentlyPressed;
  return ret;
}