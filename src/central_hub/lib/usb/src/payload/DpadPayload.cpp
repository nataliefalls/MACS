#include "DpadPayload.h"

payload_t DpadPayload::toPayload() const {
    return { .dpad = data };
}

bool DpadPayload::isEquivalent(const IPayload *other) const {
    if (other == nullptr) return false;
    // we know the type of other and this are the same from the precondition
    dpad_data_t otherDpad = other->toPayload().dpad;
    return (this->data.dpad == otherDpad.dpad);
}

report_id_t DpadPayload::reportID() const {
    return REPORT_ID_DPAD_DATA;
}