#pragma once

#include <stdint.h>
#include "Adafruit_MAX31855.h"

typedef struct {
    float temp_internal;
    float temp_external;
} thermocouple_meas_t;

struct thermocouple {
    thermocouple(uint8_t cs);
    thermocouple_meas_t get() const;
    void update();
private:
    Adafruit_MAX31855 _max31855;
    thermocouple_meas_t _curr_readout;
};

extern thermocouple temp_main;
extern thermocouple temp_aft;
