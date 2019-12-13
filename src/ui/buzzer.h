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
    void buzz(uint32_t buzz_length_ms);
    void task();

private:
    static uint32_t _count;
    static portMUX_TYPE _cnt_mutex;
};

extern Buzzer buzzer;
