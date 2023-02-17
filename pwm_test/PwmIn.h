#ifndef PwmIn_H
#define PwmIn_H

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "PwmIn.h"
#include "PwmIn.pio.h"
#include "pico/time.h"

// class that reads PWM pulses on max 4 pins
class PwmIn
{
public:
    // constructor
    PwmIn(uint *pin_list, uint num_of_pin);
    // read only the pulsewidth
    uint16_t read_PW(uint pin);

private:
    // set the irq handler
    static void pio_irq_handler()
    {
        int state_machine = -1;
        printf("Interrupt occurred");
        // check which IRQ was raised:
        for (int i = 0; i < 4; i++)
        {
            if (pio0_hw->irq & 1<<i)
            {
                // clear interrupt
                pio0_hw->irq = 1 << i;
                // read pulse width from the FIFO
                pulsewidth[i] = pio_sm_get(pio, i);
                // read low period from the FIFO
                period[i] = pio_sm_get(pio, i);
                // clear interrupt
                pio0_hw->irq = 1 << i;
            }
        }
    }
    // the pio instance
    static PIO pio;
    // the pins and number of pins
    uint _num_of_pins;
    // data about the PWM input measured in pio clock cycles
    static uint32_t pulsewidth[4], period[4];

    static bool isNew[4];
};

#endif