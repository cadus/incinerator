#pragma once

#include "ignition.h"
#include "thermocouple.h"
#include "timeout.h"

struct BurnChamber {
    BurnChamber(uint8_t ignition_pin, uint8_t thermocouple_cs);

    thermocouple_meas_t getTemp();
    void task();

    Thermocouple _thermocouple;
    Ignition _ignition;
    Timeout _tempReadTimeout;

    static constexpr uint32_t _tempReadInterval = 500;
};
