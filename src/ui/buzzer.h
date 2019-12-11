#pragma once

#include <stdint.h>

#include "macros.h"

class Buzzer
{
    NOT_COPYABLE(Buzzer);
    NOT_MOVEABLE(Buzzer);

public:
    Buzzer();

    void init();
    void buzz(uint32_t buzz_length_ms, uint8_t volume=16);
    void task();

private:
    static DRAM_ATTR uint32_t _count;
};

extern Buzzer buzzer;
