#include "thermocouple.h"

#include "Adafruit_MAX31855.h"

#include "hw_config.h"

static Adafruit_MAX31855 thermo_ch1(TEMP_CH1_CS);
static Adafruit_MAX31855 thermo_ch2(TEMP_CH2_CS);

thermocouple_meas_t thermocouple_get()
{
    return (thermocouple_meas_t) {
        .temp_internal = thermo_ch1.readInternal(),
        .temp_external = thermo_ch1.readCelsius()
    };
}
