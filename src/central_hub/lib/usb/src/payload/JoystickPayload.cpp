#include "JoystickPayload.h"

payload_t JoystickPayload::toPayload() const {
    return { .joystick = data };
}

bool JoystickPayload::isEquivalent(const IPayload *other) const {
    if (other == nullptr) return false;
    // we know the type of other and this are the same from the precondition
    joystick_data_t otherJoystick = other->toPayload().joystick;
    
    return this->isWithinTolerance(this->data.x, otherJoystick.x)
        && this->isWithinTolerance(this->data.y, otherJoystick.y)
        && this->data.button == otherJoystick.button;
}

bool JoystickPayload::isWithinTolerance(uint8_t value1, uint8_t value2) const {
    uint8_t diff = (value1 <= value2) ? (value2 - value1) : (value1 - value2);
    return diff <= JoystickPayload::TOLERANCE;
}

report_id_t JoystickPayload::reportID() const {
    return REPORT_ID_JOYSTICK_DATA;
}