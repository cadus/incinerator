#include "thermocouple.h"
#include "hw_config.h"

Thermocouple::Thermocouple(uint8_t cs)
:_max31855(cs)
{
}

void Thermocouple::init()
{
    _max31855.begin();
}

thermocouple_meas_t Thermocouple::get() const
{
    return _curr_readout;
}

void Thermocouple::update()
{
    size_t retries = 5;
    bool success = false;

    thermocouple_meas_t tmp;
    while (retries-- && !success) {
        tmp = (thermocouple_meas_t) {
        .internal = float(_max31855.readInternal()),
        .external = float(_max31855.readCelsius())
    };
        if (isnan(tmp.internal) || isnan(tmp.external)) {
            continue;
        }
        if (tmp.internal < -100 || tmp.internal > 300) {
            continue;
        }
        if (tmp.external < -100 || tmp.external > 3000) {
            continue;
        }
        success = true;
    }

    if (!success) {
        syslog(LOG_ERROR, "Thermocouple %s error (%.1f, %.1f)", _name.c_str(), tmp.internal, tmp.external);
        return;
    }

    _curr_readout = tmp;
//  syslog(LOG_DEBUG, "R = %d, T_int: %.1f, T_ext: %.1f", retries, _curr_readout.internal, _curr_readout.external);
}
