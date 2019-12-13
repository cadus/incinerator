#include "timeout.h"

volatile int32_t Timeout::_counter = 0;
portMUX_TYPE Timeout::_tm_mutex = portMUX_INITIALIZER_UNLOCKED;

Timeout::Timeout()
{
    set(0);
}

void Timeout::set(int32_t ms)
{
    _timeout = getMs() + ms;
}

bool Timeout::elapsed()
{
    return (getMs() - _timeout) >= 0;
}

void IRAM_ATTR Timeout::incrementMs()
{
    // called from interrupt
    portENTER_CRITICAL_ISR(&_tm_mutex);
    _counter++;
    portEXIT_CRITICAL_ISR(&_tm_mutex);
}

int32_t Timeout::getMs()
{
    return _counter;
}
