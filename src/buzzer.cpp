#include <Arduino.h>
#include <buzzer.h>

#include "hw_config.h"

Buzzer::Buzzer()
:_count(0)
{
}

void Buzzer::init()
{
    ledcSetup(PWM_CH_BUZZER, 4000, 8);
    ledcAttachPin(BUZZER, PWM_CH_BUZZER);
}

void Buzzer::buzz(uint32_t buzz_length_ms)
{
    if (_count < buzz_length_ms) {
        digitalWrite(LED_INT, true);
        ledcWrite(PWM_CH_BUZZER, 128);
        _count = buzz_length_ms;
    }
}

void Buzzer::task()
{
    if (_count > 0) {
        if (!--_count) {
            digitalWrite(LED_INT, false);
            ledcWrite(PWM_CH_BUZZER, 0);
        }
    }
}

Buzzer buzzer;
