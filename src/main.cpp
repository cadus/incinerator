#include <Arduino.h>
#include <TimerOne.h>

#include "hw_config.h"

static void timer_isr(void);

void setup()
{
    pinMode(ROTARY_SW, INPUT_PULLUP);
    pinMode(ROTARY_CLK, INPUT_PULLUP);
    pinMode(ROTARY_DT, INPUT_PULLUP);

    pinMode(TESTPIN1, OUTPUT);
    pinMode(TESTPIN2, OUTPUT);

    Timer1.initialize(1000);
    Timer1.attachInterrupt(timer_isr);

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

volatile bool flag = false;
volatile int encoder_pos = 0;

void loop()
{
    if (flag) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        flag = false;
    }
    static int old_encoder_pos = 0;
    if (encoder_pos != old_encoder_pos) {
        old_encoder_pos = encoder_pos;
        Serial.print(encoder_pos, DEC);
        Serial.println();
    }
}

static void check_encoder_rotation()
{
    const int8_t transition_valid_tbl[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };
    static uint8_t transition_code = 0;
    static uint8_t transition_code_history = 0;

    transition_code <<= 2;
    transition_code |= digitalRead(ROTARY_DT) ? (1<<1) : 0;
    transition_code |= digitalRead(ROTARY_CLK) ? (1<<0) : 0;
    transition_code &= 0x0f;

    if (transition_valid_tbl[transition_code]) {
        transition_code_history <<= 4;
        transition_code_history |= transition_code;

        if (transition_code_history == 0x2b) {
            encoder_pos--;
        } else if (transition_code_history == 0x17) {
            encoder_pos++;
        }
    }
}

static void check_encoder_switch(void)
{
    static bool switch_old = false;
    static uint8_t switch_history = 0;
    switch_history <<= 1;
    switch_history |= digitalRead(ROTARY_SW) ? 1 : 0;
    if (switch_history == 0x00 || switch_history == 0xff) {
        const bool switch_curr = !!switch_history;
        if (switch_curr != switch_old) {
            flag = !switch_curr;
            switch_old = switch_curr;
        }
    }
}

static void timer_isr(void)
{
    digitalWrite(TESTPIN2, HIGH);

    check_encoder_rotation();
    check_encoder_switch();

    digitalWrite(TESTPIN2, LOW);
}