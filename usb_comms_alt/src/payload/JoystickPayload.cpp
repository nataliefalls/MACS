#include "JoystickPayload.h"

payload_t JoystickPayload::toPayload() const {
    return { .joystick = data };
}

bool JoystickPayload::isEquivalent(const IPayload *other) const {
    if (other == nullptr) return false;
    // we know the type of other and this are the same from the precondition
    joystick_data_t otherJoystick = other->toPayload().joystick;
    return (this->data.x == otherJoystick.x && this->data.y == otherJoystick.y);
}

report_id_t JoystickPayload::reportID() const {
    return REPORT_ID_JOYSTICK_DATA;
}