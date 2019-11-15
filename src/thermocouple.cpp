#include "thermocouple.h"

#include "Adafruit_MAX31855.h"

#include "hw_config.h"

static Adafruit_MAX31855 thermocouple(MAX31855_CS);

thermocouple_meas_t thermocouple_get()
{
    return (thermocouple_meas_t){
        .temp_internal = thermocouple.readInternal(),
        .temp_external = thermocouple.readCelsius()
    };
}
