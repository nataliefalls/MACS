#include "PicoTimer.h"
#include "pico/time.h"

void PicoTimer::startTimer(uint32_t timeout) {
    this->time = time_us_32();
    this->timeout = timeout;
}

bool PicoTimer::timeoutOccured() const {
    uint32_t elapsed = time_us_32() - this->time;
    return elapsed >= this->timeout;
}