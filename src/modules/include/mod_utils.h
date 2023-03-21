/**
 * Testing out preprocessor directives that can be set in CMake with target_compile_definitions for different targets
*/

#ifndef MOD_UTILS
#define MOD_UTILS

#include <pico/stdlib.h>
#include <cstdio>
#include <hardware/i2c.h>
#include <common.h>

#ifndef PARAM_ASSERTIONS_ENABLED_MODULE
#define PARAM_ASSERTIONS_ENABLED_MODULE 0
#endif

namespace module {

uint8_t get_address();

}

#endif