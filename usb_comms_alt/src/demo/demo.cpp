#include "bsp/board.h"
#include "hardware/adc.h"

#include "demo.h"
#include "report_types.h"
#include "ReportQueueController.h"
#include "ReportQueueHandler.h"
#include "PicoQueueReportQueue.h"

#include "Module.h"
#include "ButtonPayload.h"
#include "DpadPayload.h"

#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

queue_t sharedQueue;
IReportQueue *queue = new PicoQueueReportQueue(&sharedQueue);
ReportQueueHandler *handler = new ReportQueueHandler(queue);
ReportQueueController *controller = new ReportQueueController(queue);

Module<ButtonPayload> *module;
const uint8_t moduleID = 0x68;

void connectModule() {
  module_coordinates_t coordinates = { 3, 3 };
  // controller->moduleConnectedReport(moduleID, coordinates);

  // return;
  module = new Module<ButtonPayload>(moduleID, coordinates, controller);
}

void updateModule0() {
  ButtonPayload *button0 = new ButtonPayload({ 0 });
  module->update(button0);
}
void updateModule1() {
  ButtonPayload *button1 = new ButtonPayload({ 1 });
  module->update(button1);
}

void updateModule(uint8_t state) {
  ButtonPayload *button = new ButtonPayload({ state });
  module->update(button);
}

void updateModule() {
  ButtonPayload *button = new ButtonPayload({ read_button(BUTTON_PIN) });
  module->update(button);
}

void removeModule() {
  delete module;
}

void send_demo_report() {
  static uint8_t count = 0;

  switch (count) {
    case 0: {
      connectModule();
      count++;
      break;
    }
    case 1: {
      updateModule0();
      count++;
      break;
    }
    case 2: {
      updateModule0();
      count++;
      break;
    }
    case 3: {
      updateModule1();
      count++;
      break;
    }
    case 4: {
      updateModule1();
      count++;
      break;
    }
    case 5: {
      removeModule();
      count = 0;
      break;
    }
  }

  handler->sendNextReport();
}

void init_adc_gpio() {
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_down(BUTTON_PIN);

  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
}

inline uint16_t adc_read_pin(uint pin) {
  adc_select_input(pin);
  return adc_read();
}

inline uint8_t read_button(uint pin) {
  return gpio_get(pin) ? 1 : 0;
}