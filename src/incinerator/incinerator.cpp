#include "incinerator.h"

#include "hw_config.h"

#include "util/syslog.h"

Incinerator::Incinerator()
:_burner_main("MAIN", IGNITION_1, TEMP_CH1_CS)
,_burner_aft("AFT", IGNITION_2, TEMP_CH2_CS)
,_airPump(AIRPMP, PWM_CH_AIRPMP)
{
}

void Incinerator::init()
{
    _burner_main.init();
    _burner_aft.init();
    _airPump.init();
}

void Incinerator::task()
{
    _burner_main.task();
    _burner_aft.task();
}

thermocouple_meas_t Incinerator::getTemp(burn_chamber_t ch)
{
    switch (ch) {
    default:
        syslog(LOG_ERROR, "Unknown burn chamber %d", ch);
        return (thermocouple_meas_t){0, 0};
    case chamber_main:
        return _burner_main.getTemp();
    case chamber_aft:
        return _burner_aft.getTemp();
    }
}

Incinerator incinerator;
