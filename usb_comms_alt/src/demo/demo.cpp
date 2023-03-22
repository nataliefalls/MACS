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

void connectModule() {
  uint8_t moduleID = 0x68;
  module_coordinates_t coordinates = { 3, 3 };
  module = new Module<ButtonPayload>(moduleID, coordinates, controller);
}

void updateModule() {
  uint8_t state = gpio_get(BUTTON_PIN) ? 1 : 0;

  button_data_t data = { state };
  ButtonPayload *dpad = new ButtonPayload(data);
  module->update(dpad);
}

void removeModule() {
  delete module;
}

void send_demo_report() {
  static int count = 0;

  if (count == 0) {
    connectModule();
    handler->sendNextReport();
    count++;
  } else if (count == 1 || count == 2) {
    updateModule();
    handler->sendNextReport();
    count++;
  } else { 
    removeModule();
    handler->sendNextReport();
    count = 0;
  }
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