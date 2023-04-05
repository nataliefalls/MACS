#include "demoUtils.h"
#include "gpioUtils.h"

ModuleUpdateHandler<ButtonPayload>* connectButtonModule(IForceableReportQueueController *controller);
ModuleUpdateHandler<JoystickPayload>* connectJoystickModule(IForceableReportQueueController *controller);
void removeButtonModule(ModuleUpdateHandler<ButtonPayload> *buttonModule);
void removeJoystickModule(ModuleUpdateHandler<JoystickPayload> *joystickModule);

ModuleUpdateHandler<ButtonPayload>* connectButtonModule(IForceableReportQueueController *controller) {
  return new ModuleUpdateHandler<ButtonPayload>(buttonModuleID, { 0, 1 }, controller);
}

ModuleUpdateHandler<JoystickPayload>* connectJoystickModule(IForceableReportQueueController *controller) {
  return new ModuleUpdateHandler<JoystickPayload>(joystickModuleID, { 1, 1 }, controller);
}

bool updateButtonModule(ModuleUpdateHandler<ButtonPayload> *buttonModule) {
  ButtonPayload *button = new ButtonPayload({ read_button(BUTTON_PIN) });
  if (!buttonModule->update(button)) {
    // if the update didn't go through, the module no longer keeps
    // track of this payload. since we're finished with it,
    // we need to free the memory
    delete button;
    return false;
  }
  return true;
}

bool updateJoystickModule(ModuleUpdateHandler<JoystickPayload> *joystickModule) {
  JoystickPayload *joystick = new JoystickPayload({
    adc_read_pin(JOYSTICK_X_PIN),
    adc_read_pin(JOYSTICK_Y_PIN),
    read_button(JOYSTICK_BUTTON_PIN),
  });
  if (!joystickModule->update(joystick)) {
    // if the update didn't go through, the module no longer keeps
    // track of this payload. since we're finished with it,
    // we need to free the memory
    delete joystick;
    return false;
  }
  return true;
}

void removeButtonModule(ModuleUpdateHandler<ButtonPayload> *buttonModule) {
  delete buttonModule;
}

void removeJoystickModule(ModuleUpdateHandler<JoystickPayload> *joystickModule) {
  delete joystickModule;
}

bool toggleButtonConnection(ModuleUpdateHandler<ButtonPayload> **buttonModule, IForceableReportQueueController *controller, bool &connected) {
  static bool currentlyPressed = false;
  static bool previouslyPressed = false;
  bool ret = false;

  currentlyPressed = read_button(MODULE_CONNECT_PIN) != 0;

  // triggers only on positive edge of button
  // this way, it won't toggle connection if we hold down the button
  if (currentlyPressed && !previouslyPressed) {
    if (connected) {
        removeButtonModule(*buttonModule);
    } else {
        *buttonModule = connectButtonModule(controller);
    }
    connected = !connected;
    ret = true;
  }
  previouslyPressed = currentlyPressed;
  return ret;
}

bool toggleJoystickConnection(ModuleUpdateHandler<JoystickPayload> **joystickModule, IForceableReportQueueController *controller, bool &connected) {
  static bool currentlyPressed = false;
  static bool previouslyPressed = false;
  bool ret = false;

  currentlyPressed = read_button(MODULE_CONNECT_PIN) != 0;

  // triggers only on positive edge of button
  // this way, it won't toggle connection if we hold down the button
  if (currentlyPressed && !previouslyPressed) {
    if (connected) {
        removeJoystickModule(*joystickModule);
    } else {
        *joystickModule = connectJoystickModule(controller);
    }
    connected = !connected;
    ret = true;
  }
  previouslyPressed = currentlyPressed;
  return ret;
}