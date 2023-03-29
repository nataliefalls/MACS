#include "JoystickPayload.h"

payload_t JoystickPayload::toPayload() const {
    return { .joystick = data };
}

bool JoystickPayload::isEquivalent(const IPayload *other) const {
    if (other == nullptr) return false;
    // we know the type of other and this are the same from the precondition
    joystick_data_t otherJoystick = other->toPayload().joystick;
    return (this->isWithinTolerance(this->data.x, otherJoystick.x) &&
        this->isWithinTolerance(this->data.y, otherJoystick.y) &&
        this->data.button == otherJoystick.button);
}

report_id_t JoystickPayload::reportID() const {
    return REPORT_ID_JOYSTICK_DATA;
}

bool JoystickPayload::isWithinTolerance(uint8_t x, uint8_t y) const {
    return (x > y) && (x - y <= this->TOLERANCE)
        || (x <= y) && (y - x <= this->TOLERANCE);
}