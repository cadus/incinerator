#include "thermocouple.h"
#include "hw_config.h"

Thermocouple::Thermocouple(uint8_t cs)
:_max31855(cs)
{
}

thermocouple_meas_t Thermocouple::get() const
{
    return _curr_readout;
}

void Thermocouple::update()
{
    _curr_readout = (thermocouple_meas_t) {
        .internal = float(_max31855.readInternal()),
        .external = float(_max31855.readCelsius())
    };
}
