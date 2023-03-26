#pragma once
#include <stdint.h>

class PicoTimer {
    private:
    uint32_t time;
    uint32_t timeout;

    public:
    /**
     * start the timer with the given duration in microseconds
    */
    void startTimer(uint32_t duration);

    /**
     * has a previously set timeout occured?
    */
    bool timeoutOccured() const;
};