#pragma once

#include <Arduino.h>
#include <stdint.h>

class Timeout
{
public:
    Timeout();

    void set(int32_t ms);
    bool elapsed();

    static int32_t getMs();
    static void incrementMs();

private:
    static volatile int32_t _counter;
    int32_t _timeout;
    static portMUX_TYPE _tm_mutex;
};
