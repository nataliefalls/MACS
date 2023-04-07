#include "AnalogPayload.h"

payload_t AnalogPayload::toPayload() const {
    return { .analog = data };
}

bool AnalogPayload::isEquivalent(const IPayload *other) const {
    if (other == nullptr) return false;
    // we know the type of other and this are the same from the precondition
    analog_data_t otherAnalog = other->toPayload().analog;
    return (this->data.analog == otherAnalog.analog);
}

report_id_t AnalogPayload::reportID() const {
    return REPORT_ID_ANALOG_DATA;
}