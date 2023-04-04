/**
 * Testing out preprocessor directives that can be set in CMake with target_compile_definitions for different targets
*/

#ifndef MOD_UTILS
#define MOD_UTILS

#include <pico/stdlib.h>
#include <cstdio>
#include <hardware/i2c.h>
#include <common.h>

#if ANALOG
#include <hardware/adc.h>
#endif

#ifndef PARAM_ASSERTIONS_ENABLED_MODULE
#define PARAM_ASSERTIONS_ENABLED_MODULE 0
#endif

/**
 * @brief This namespace includes all necessary functions specific to the modules.
*/
namespace module {

/**
 * @brief Maps a 16 bit value to an 8-bit scale, reducing resolution of analog value but saving message space.
 * 
 * @param x Input 16 bit value
 * @return Mapped 8 bit value
*/
uint8_t map_16_to_8(uint16_t x);

/**
 * @brief Returns the I2C address of this module.
*/
uint8_t get_address();

#if DIGITAL
/**
 * @brief Returns the GPIO pin the digital input is connected to.
*/
uint get_digital_pin();
#endif

/**
 * @brief Initializes the relevant GPIO pins to receive input for this module.
*/
void init_inputs();

/**
 * @brief Fetches necessary input values for this module.
 * 
 * @param buf Buffer to place input values into
 * @return Size of input placed in buf
*/
uint8_t get_input(uint8_t* buf);



}

#endif