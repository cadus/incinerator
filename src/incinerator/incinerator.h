#include "air_pump.h"
#include "burn_chamber.h"
#include "macros.h"

enum burn_chamber_t {
    chamber_main,
    chamber_aft
};

class Incinerator
{
    NOT_COPYABLE(Incinerator);
    NOT_MOVEABLE(Incinerator);

public:
    Incinerator();
    void init();
    void task();

    thermocouple_meas_t getTemp(burn_chamber_t ch);

    BurnChamber _burner_main;
    BurnChamber _burner_aft;
    AirPump _airPump;
};

extern Incinerator incinerator;
