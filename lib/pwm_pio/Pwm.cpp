#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"

#include "Pwm.h"
#include "Pwm.pio.h"

// class that reads PWM pulses from up to 4 pins
Pwm::Pwm(uint pwm_out, uint *pin_in_list, uint num_of_pins)
{
  _num_of_pins = num_of_pins;

  _pwm_out = pwm_out;
    
  // load the pio program into the pio memory
  uint offset1 = pio_add_program(pio0, &Pwm_program);
  uint offset2 = pio_add_program(pio1, &Pwm_program);
  pio_sm_config c1 = Pwm_program_get_default_config(offset1);
  pio_sm_config c2 = Pwm_program_get_default_config(offset2);
    
  // start num_of_pins state machines
  for (int ii = 0; ii < num_of_pins; ii++)
    {
      // use both pio's if necessary
      pio = ii < 4 ? pio0 : pio1;
      uint offset = ii < 4 ? offset1 : offset2;
      // make a sm config
      pio_sm_config c = ii < 4 ? c1 : c2;

      int i = ii % 4;
      

      // prepare state machine i
      pulsewidth[ii] = 0;
      period[ii] = 0;

      // configure the used pins (pull down, controlled by PIO)
      gpio_pull_down(pin_list[ii]);
      pio_gpio_init(pio, pin_list[ii]);
      // set the 'jmp' pin
      sm_config_set_jmp_pin(&c, pin_list[ii]);
      // set the 'wait' pin (uses 'in' pins)
      sm_config_set_in_pins(&c, pin_list[ii]);
      // set shift direction
      sm_config_set_in_shift(&c, false, false, 0);
      // init the pio sm with the config
      pio_sm_init(pio, i, offset, &c);
      // enable the sm
      pio_sm_set_enabled(pio, i, true);
    }
  // set the IRQ handler
  irq_set_exclusive_handler(PIO0_IRQ_0, pio0_irq_handler);
  // enable the IRQ
  irq_set_enabled(PIO0_IRQ_0, true);

  // set the IRQ handler
  irq_set_exclusive_handler(PIO1_IRQ_0, pio1_irq_handler);
  // enable the IRQ
  irq_set_enabled(PIO1_IRQ_0, true);
  // allow irqs from the low 4 state machines
  pio0_hw->inte0 = PIO_IRQ0_INTE_SM0_BITS | PIO_IRQ0_INTE_SM1_BITS | PIO_IRQ0_INTE_SM2_BITS | PIO_IRQ0_INTE_SM3_BITS ;
  pio1_hw->inte0 = PIO_IRQ1_INTE_SM0_BITS | PIO_IRQ1_INTE_SM1_BITS | PIO_IRQ1_INTE_SM2_BITS | PIO_IRQ1_INTE_SM3_BITS ;
};

uint16_t Pwm::read_PW(uint pin)
{
  // the measurements are taken with 2 clock cycles per timer tick
  // hence, it is 2*0.000000008
  //printf("\nPulsewidth is: %d\n", (pulsewidth[pin]));
  isNew[pin] = false;
  return pulsewidth[pin];
};

bool Pwm::isConnected(uint pin)
{
  return isNew[pin];
};

void PWmIn::setPWMOut(uint16_t value)
{
  gpio_set_function(_pwm_out, GPIO_FUNC_PWM);
  
}

uint32_t Pwm::pulsewidth[8];
uint32_t Pwm::period[8];
PIO Pwm::pio;
bool Pwm::isNew[8];
uint Pwm::_pwm_out;
