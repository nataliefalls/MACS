#include <mod_utils.h>
#include <hardware/gpio.h>

uint8_t read_digital_input(uint pin) {
    if(gpio_get_dir(pin) == GPIO_IN) {
        return gpio_get(pin);
    }
    return UINT8_MAX;
}

#if ID_1
// proof of concept stuff
void test_id_1() {
    printf("coolio\n");
}
#endif