#include "bsp/board.h"
#include "hardware/adc.h"

#include "demo.h"
#include "report_types.h"
#include "ReportQueueController.h"
#include "ReportQueueHandler.h"
#include "MulticoreReportQueue.h"

#include "Module.h"
#include "ButtonPayload.h"

#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

IReportQueue *queue = new MulticoreReportQueue();
ReportQueueHandler *handler = new ReportQueueHandler(queue);
ReportQueueController *controller = new ReportQueueController(queue);

Module<ButtonPayload> *module;

void connectModule() {
  uint8_t moduleID = 0x68;
  module_coordinates_t coordinates = { 3, 3 };
  module = new Module<ButtonPayload>(0x68, coordinates, controller);
}

void updateModule() {
  static bool pressed = false;
  static int count = 0;

  button_data_t data = { (uint8_t) (pressed ? 1 : 0) };
  IPayload *button = new ButtonPayload(data);
  if (module) {
    module->update(button);
  }

  if (++count % 1000 == 0) pressed == !pressed;
  if (count >= 6000) count = 0;
}

void removeModule() {
  delete module;
  module = nullptr;
}

void init_adc_gpio() {
  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_down(BUTTON_PIN);

  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);
}

static inline uint16_t adc_read_pin(uint pin) {
  adc_select_input(pin);
  return adc_read();
}

void send_demo_report() {
  static int count = 0;

  if (count == 0) {
    connectModule();
  } else if (count <= 7000) { 
    updateModule();
  } else if (count == 8000) { 
    removeModule();
  }

  handler->sendNextReport();

  if (++count >= 9000) {
    count = 0;
  }
}