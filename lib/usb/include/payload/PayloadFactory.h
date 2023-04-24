#pragma once

#include <stdint.h>
#include "IPayload.h"

class PayloadFactory {
    public:
    /**
     * Construct a digital payload with the given data.
     * 
     * @param digitalValue the data to construct the payload out of
     * @returns a payload containing the given data
    */
    static IPayload* digitalPayload(uint8_t digitalValue);

    /**
     * Construct a digital payload with the given data.
     * 
     * @param analogValue the data to construct the payload out of
     * @returns a payload containing the given data
    */
    static IPayload* analogPayload(uint8_t analogValue);

    /**
     * Construct a digital payload with the given data.
     * 
     * @param x the x component of the joystick data
     * @param y the y component of the joystick data
     * @param button the button component of the joystick data
     * @returns a payload containing the given data
    */
    static IPayload* joystickPayload(uint8_t x, uint8_t y, uint8_t button);
};