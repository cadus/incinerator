#include "burn_chamber.h"

#include "macros.h"

class Incinerator
{
    NOT_COPYABLE(Incinerator);
    NOT_MOVEABLE(Incinerator);

public:
    Incinerator();
    void task();
};

extern Incinerator incinerator;
extern BurnChamber burner_main;
extern BurnChamber burner_aft;
