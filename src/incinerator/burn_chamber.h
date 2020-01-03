#pragma once

#include <string>

#include "ignition.h"
#include "thermocouple.h"
#include "util/timeout.h"

#include "macros.h"

class BurnChamber
{
    NOT_COPYABLE(BurnChamber);
    NOT_MOVEABLE(BurnChamber);

public:
    BurnChamber(std::string name, uint8_t ignition_pin, uint8_t thermocouple_cs);

    thermocouple_meas_t getTemp() const;

    void init();
    void task();

private:
    const std::string _name;
    Thermocouple _thermocouple;
    Ignition _ignition;
    Timeout _tempReadTimeout;

    static constexpr uint32_t _tempReadInterval = 500;
};
