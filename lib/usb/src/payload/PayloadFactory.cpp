#include "PayloadFactory.h"

#include "DigitalPayload.h"
#include "AnalogPayload.h"
#include "JoystickPayload.h"

IPayload* PayloadFactory::analogPayload(uint8_t analogValue) {
    return new AnalogPayload({ analogValue });
}

IPayload* PayloadFactory::digitalPayload(uint8_t digitalValue) {
    return new DigitalPayload({ digitalValue });
}

IPayload* PayloadFactory::joystickPayload(uint8_t x, uint8_t y, uint8_t button) {
    return new JoystickPayload({ x, y, button });
}

