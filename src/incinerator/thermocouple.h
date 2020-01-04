#pragma once

#include <stdint.h>
#include <string>

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
    Thermocouple(std::string name, uint8_t cs);
    thermocouple_meas_t get() const;
    void init();
    void update();

private:
    const std::string _name;
    Adafruit_MAX31855 _max31855;
    thermocouple_meas_t _curr_readout;
};
