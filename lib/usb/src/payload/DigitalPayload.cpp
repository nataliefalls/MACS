#include "DigitalPayload.h"

payload_t DigitalPayload::toPayload() const {
    return { .digital = data };
}

bool DigitalPayload::sameData(const IPayload *other) const {
    digital_data_t otherDigital = other->toPayload().digital;
    return (this->data.digital == otherDigital.digital);
}

report_id_t DigitalPayload::reportID() const {
    return REPORT_ID_DIGITAL_DATA;
}