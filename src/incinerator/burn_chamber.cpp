#include "incinerator/burn_chamber.h"

BurnChamber::BurnChamber(std::string name, uint8_t ignition_pin, uint8_t thermocouple_cs)
: _name(name)
, _thermocouple(name, thermocouple_cs)
, _ignition(name, ignition_pin, _thermocouple)
{
}

thermocouple_meas_t BurnChamber::getTemp() const
{
    return _thermocouple.get();
}

void BurnChamber::init()
{
    _thermocouple.init();
}

void BurnChamber::task()
{
    if (_tempReadTimeout.elapsed()) {
        _tempReadTimeout.set(_tempReadInterval);
        _thermocouple.update();
    }
    _ignition.task();
}
