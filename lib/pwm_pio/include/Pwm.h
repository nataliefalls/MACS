#ifndef Pwm_H
#define Pwm_H

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "Pwm.pio.h"
#include "pico/time.h"

// class that reads PWM pulses on max 4 pins
class Pwm
{
public:
  // constructor
  Pwm(uint *pin_in_list, uint num_of_pins);
  // read only the pulsewidth
  uint16_t read_PW(uint pin);
  bool isConnected(uint pin);
  void setPWMOut(uint16_t value);

private:
  // set the irq handler
  static void pio0_irq_handler()
  {
    int state_machine = -1;
    //printf("Interrupt occurred");
    // check which IRQ was raised:
	
    for (int i = 0; i < 4; i++)
      {
	if (pio0_hw->irq & 1<<i)
	  {
            // clear interrupt
            pio0_hw->irq = 1 << i;
            // read pulse width from the FIFO
            pulsewidth[i] = pio_sm_get(pio0, i);
            // read low period from the FIFO
            period[i] = pio_sm_get(pio0, i);
            // clear interrupt
            pio0_hw->irq = 1 << i;
            isNew[i] = true;
            return;
	  }
      }
  }

  static void pio1_irq_handler()
  {
    int state_machine = -1;
    //printf("Interrupt occurred");
    // check which IRQ was raised:
      
    for (int i = 0; i < 4; i++) {
      if (pio1_hw->irq & 1<<i) {
	// clear interrupt
	pio0_hw->irq = 1 << i;
	// read pulse width from the FIFO
	pulsewidth[i + 4] = pio_sm_get(pio1, i);
	// read low period from the FIFO
	period[i + 4] = pio_sm_get(pio1, i);
	// clear interrupt
	pio1_hw->irq = 1 << i;
	isNew[i + 4] = true;
	return;
      }
    }
  }
  // the pio instance
  static PIO pio;
  // output pin
  uint _pwm_out;
  // the pins and number of pins
  uint _num_of_pins;
  // data about the PWM input measured in pio clock cycles
  static uint32_t pulsewidth[8], period[8];

  static bool isNew[8];
};

#endif
