#include "ButtonPayload.h"

payload_t ButtonPayload::toPayload() const {
    return { .button = data };
}

bool ButtonPayload::isEqual(const IPayload& other) const {
    // we know the type of other and this are the same from the precondition
    button_data_t otherButton = other.toPayload().button;
    return (this->data.button == otherButton.button);
}

report_id_t ButtonPayload::reportID() const {
    return REPORT_ID_BUTTON_DATA;
}