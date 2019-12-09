#include "incinerator.h"

#include "hw_config.h"

BurnChamber burner_main(IGNITION_1, TEMP_CH1_CS);
BurnChamber burner_aft(IGNITION_2, TEMP_CH2_CS);
