/**
 * Testing out preprocessor directives that can be set in CMake with target_compile_definitions for different targets
*/

#include <cstdint>

void test(uint8_t *test) {
#if BUTTON | SWITCH
    //this piece of code will run if the module is a digital input
#endif

#if POTENTIOMETER
    //this piece of code will run if the module is an analog input
#endif
}