#pragma once

#include <stdint.h>

struct Buzzer {
    Buzzer();

    void init();
    void buzz(uint32_t buzz_length_ms);
    void task();

private:
    uint32_t _count;
};

extern Buzzer buzzer;
