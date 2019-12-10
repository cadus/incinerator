#pragma once

#include <stdint.h>
#include "Adafruit_MAX31855.h"

#include "macros.h"

typedef struct {
    float internal;
    float external;
} thermocouple_meas_t;

class Thermocouple
{
    NOT_COPYABLE(Thermocouple);
    NOT_MOVEABLE(Thermocouple);

public:
    Thermocouple(uint8_t cs);
    thermocouple_meas_t get() const;
    void update();
private:
    Adafruit_MAX31855 _max31855;
    thermocouple_meas_t _curr_readout;
};

