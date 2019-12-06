#include "thermocouple.h"
#include "hw_config.h"

thermocouple temp_main(TEMP_CH1_CS);
thermocouple temp_aft(TEMP_CH2_CS);

thermocouple::thermocouple(uint8_t cs)
:_max31855(cs)
{
}

thermocouple_meas_t thermocouple::get() const
{
    return _curr_readout;
}

void thermocouple::update()
{
    _curr_readout = (thermocouple_meas_t) {
        .temp_internal = _max31855.readInternal(),
        .temp_external = _max31855.readCelsius()
    };
}
