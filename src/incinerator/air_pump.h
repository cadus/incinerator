#pragma once

#include <stdint.h>
#include "macros.h"

class AirPump
{
    NOT_COPYABLE(AirPump);
    NOT_MOVEABLE(AirPump);

public:
    AirPump(uint8_t pin, uint8_t pwm_ch);

    void init();
    void on();
    void off();

private:
    const uint8_t _pin;
    const uint8_t _pwm_ch;
};
