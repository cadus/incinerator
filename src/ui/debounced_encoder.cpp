#include <Arduino.h>

#include "hw_config.h"

#define ALPS_ENCODER

volatile DRAM_ATTR bool encoder_sw = false;
volatile DRAM_ATTR int encoder_pos = 0;

void encoder_init()
{
    pinMode(ROTARY_SW, INPUT_PULLUP);
    pinMode(ROTARY_CLK, INPUT_PULLUP);
    pinMode(ROTARY_DT, INPUT_PULLUP);
}

bool encoder_switch()
{
    return encoder_sw;
}

int encoder_position()
{
    return encoder_pos;
}

void IRAM_ATTR encoder_check_rotation()
{
    static DRAM_ATTR int8_t transition_valid_tbl[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };
    static DRAM_ATTR uint8_t transition_code = 0;
    static DRAM_ATTR uint8_t transition_code_history = 0;

    transition_code <<= 2;
    transition_code |= digitalRead(ROTARY_DT) ? (1 << 1) : 0;
    transition_code |= digitalRead(ROTARY_CLK) ? (1 << 0) : 0;
    transition_code &= 0x0f;

    if (transition_valid_tbl[transition_code]) {
        transition_code_history <<= 4;
        transition_code_history |= transition_code;

#ifdef ALPS_ENCODER
        if ((transition_code_history == 0x2b)
            || (transition_code_history == 0xd4)) {
            encoder_pos--;
        } else if ((transition_code_history == 0x17)
                   || (transition_code_history == 0xe8)) {
            encoder_pos++;
        }
#else
        if (transition_code_history == 0x2b) {
            encoder_pos--;
        } else if (transition_code_history == 0x17) {
            encoder_pos++;
        }
#endif
    }
}

void IRAM_ATTR encoder_check_switch(void)
{
    static DRAM_ATTR uint8_t switch_history = 0;

    switch_history <<= 1;
    switch_history |= digitalRead(ROTARY_SW) ? 1 : 0;
    if (switch_history == 0x00 || switch_history == 0xff) {
        const bool switch_curr = !!switch_history;
        encoder_sw = !switch_curr;
    }
}
