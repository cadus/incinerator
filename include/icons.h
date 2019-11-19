#pragma once

#include <stdint.h>

#ifdef __AVR__
#include "avr/pgmspace.h"
#else
#define PROGMEM
#endif

// Auto-generated by scripts/create_icons.sh - do not edit

#define ICON_BOX_CLEAR_WIDTH 16
#define ICON_BOX_CLEAR_HEIGHT 16
extern const PROGMEM uint8_t ICON_BOX_CLEAR_DATA[];

#define ICON_BOX_FILLED_WIDTH 16
#define ICON_BOX_FILLED_HEIGHT 16
extern const PROGMEM uint8_t ICON_BOX_FILLED_DATA[];

