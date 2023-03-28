#ifndef COORDINATES_H
#define COORDINATES_H

#include <stdint.h>

// the coordinates of a newly connected module
typedef struct module_coordinates_t {
    // the moduleID of the neighbor that this module was connected to
    uint8_t neighbor;
    // the encoded (neighbor's) side that this module was connected to (0-5)
    uint8_t side;
} module_coordinates_t;

#endif