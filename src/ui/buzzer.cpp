#include <Arduino.h>

#include "hw_config.h"
#include "ui/buzzer.h"
#include "util/sys_config.h"

DRAM_ATTR uint32_t Buzzer::_count;

Buzzer::Buzzer()
{
}

void Buzzer::init()
{
    ledcSetup(PWM_CH_BUZZER, 4000, 8);
    ledcAttachPin(BUZZER, PWM_CH_BUZZER);
    pinMode(LED_INT, OUTPUT);
    _count = 0;
}

void Buzzer::buzz(uint32_t buzz_length_ms)
{
    if (_count < buzz_length_ms) {
        digitalWrite(LED_INT, true);
        ledcWrite(PWM_CH_BUZZER, sysconfig.get("buzzer_vol") << 4);
        _count = buzz_length_ms;
    }
}

void IRAM_ATTR Buzzer::task()
{
    if (_count > 0) {
        if (!--_count) {
            digitalWrite(LED_INT, false);
            ledcWrite(PWM_CH_BUZZER, 0);
        }
    }
}

Buzzer buzzer;
