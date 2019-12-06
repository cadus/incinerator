#pragma once

#include <stdint.h>

struct Buzzer {
    Buzzer();

    void init();
    void buzz(uint32_t buzz_length_ms, uint8_t volume=16);
    void task();

private:
    uint32_t _count;
};

extern Buzzer buzzer;
