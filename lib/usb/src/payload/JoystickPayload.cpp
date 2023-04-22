#include "JoystickPayload.h"

payload_t JoystickPayload::toPayload() const {
    return { .joystick = data };
}

bool JoystickPayload::sameData(const IPayload *other) const {
    joystick_data_t otherJoystick = other->toPayload().joystick;
    return this->isWithinTolerance(this->data.x, otherJoystick.x)
        && this->isWithinTolerance(this->data.y, otherJoystick.y)
        && this->data.digital == otherJoystick.digital;
}

bool JoystickPayload::isWithinTolerance(uint8_t value1, uint8_t value2) const {
    return (value1 >= value2) ?
        (value1 - value2 <= JoystickPayload::TOLERANCE) :
        (value2 - value1 <= JoystickPayload::TOLERANCE);
}

report_id_t JoystickPayload::reportID() const {
    return REPORT_ID_JOYSTICK_DATA;
}