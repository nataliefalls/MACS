/**
 * Testing out preprocessor directives that can be set in CMake with target_compile_definitions for different targets
*/

#ifndef INPUTS
#define INPUTS

#include <pico/stdlib.h>
#include <cstdio>

void test(uint8_t *test) {
#if DIGITAL
    printf("Digital input\n");
#endif

#if ANALOG
    //this piece of code will run if the module is an analog input
    printf("Analog input\n");
#endif
}

#if ID_1
void test_id_1();
#endif

uint8_t read_digital_input(uint pin);

#endif