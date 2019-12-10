#pragma once

#include "ignition.h"
#include "thermocouple.h"
#include "util/timeout.h"

#include "macros.h"

class BurnChamber
{
    NOT_COPYABLE(BurnChamber);
    NOT_MOVEABLE(BurnChamber);

public:
    BurnChamber(uint8_t ignition_pin, uint8_t thermocouple_cs);

    thermocouple_meas_t getTemp() const;
    void task();

private:
    Thermocouple _thermocouple;
    Ignition _ignition;
    Timeout _tempReadTimeout;

    static constexpr uint32_t _tempReadInterval = 500;
};
