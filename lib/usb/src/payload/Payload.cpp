#include "Payload.h"

bool Payload::isEquivalent(const IPayload *other) const {
    return other != nullptr
        && this->reportID() == other->reportID()
        && this->sameData(other);
}