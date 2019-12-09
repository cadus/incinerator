#include "incinerator/burn_chamber.h"

BurnChamber::BurnChamber(uint8_t ignition_pin, uint8_t thermocouple_cs)
:_thermocouple(thermocouple_cs)
,_ignition(ignition_pin, _thermocouple)
{
}

thermocouple_meas_t BurnChamber::getTemp()
{
    return _thermocouple.get();
}

void BurnChamber::task()
{
    if (_tempReadTimeout.elapsed()) {
        _tempReadTimeout.set(_tempReadInterval);
        _thermocouple.update();
    }
    _ignition.task();
}
