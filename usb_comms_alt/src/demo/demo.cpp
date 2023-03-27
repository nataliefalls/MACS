#include "bsp/board.h"
#include "hardware/adc.h"

#include "demo.h"
#include "report_types.h"
#include "ReportQueueController.h"
#include "ReportQueueHandler.h"
#include "PicoQueueReportQueue.h"

#include "ModuleUpdateHandler.h"
#include "ButtonPayload.h"

#define BUTTON_PIN 14
#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

queue_t sharedQueue;
IReportQueue *queue = new PicoQueueReportQueue(&sharedQueue);
ReportQueueHandler *handler = new ReportQueueHandler(queue);
ReportQueueController *controller = new ReportQueueController(queue);

ModuleUpdateHandler<ButtonPayload> *module;
const uint8_t moduleID = 0x68;

void connectModule() {
  module_coordinates_t coordinates = { 3, 3 };
  module = new ModuleUpdateHandler<ButtonPayload>(moduleID, coordinates, controller);
}

bool updateModule() {
  ButtonPayload *button = new ButtonPayload({ read_button(BUTTON_PIN) });
  return module->update(button);
}

void removeModule() {
  delete module;
}

void send_demo_report() {
  static bool connected = false;
  static uint8_t count = 0;

  uint8_t button = read_button(BUTTON_PIN);

  if (!connected && button) {
    connectModule();
    connected = true;
    return;
  }

  if (connected && updateModule() && button) {
    count++;
    return;
  }

  if (count == 6) {
    removeModule();
    connected = false;
    count = 0;
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