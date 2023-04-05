#include "DigitalPayload.h"

payload_t DigitalPayload::toPayload() const {
    return { .digital = data };
}

bool DigitalPayload::isEquivalent(const IPayload *other) const {
    if (other == nullptr) return false;
    // we know the type of other and this are the same from the precondition
    digital_data_t otherDigital = other->toPayload().digital;
    return (this->data.digital == otherDigital.digital);
}

report_id_t DigitalPayload::reportID() const {
    return REPORT_ID_DIGITAL_DATA;
}