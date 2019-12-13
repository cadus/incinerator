#include <Arduino.h>

#include "hw_config.h"
#include "ui/buzzer.h"
#include "util/sys_config.h"

uint32_t Buzzer::_count = 0;
portMUX_TYPE Buzzer::_cnt_mutex = portMUX_INITIALIZER_UNLOCKED;

Buzzer::Buzzer()
{
}

void Buzzer::init()
{
    ledcSetup(PWM_CH_BUZZER, 4000, 8);
    ledcAttachPin(BUZZER, PWM_CH_BUZZER);
    pinMode(LED_INT, OUTPUT);
}

void Buzzer::buzz(uint32_t buzz_length_ms)
{
    if (_count < buzz_length_ms) {
        digitalWrite(LED_INT, true);
        ledcWrite(PWM_CH_BUZZER, sysconfig.get("buzzer_vol") << 4);
        portENTER_CRITICAL(&_cnt_mutex);
        _count = buzz_length_ms;
        portENTER_CRITICAL(&_cnt_mutex);
    }
}

void IRAM_ATTR Buzzer::task()
{
    portENTER_CRITICAL_ISR(&_cnt_mutex);
    if (_count > 0) {
        if (!--_count) {
            digitalWrite(LED_INT, false);
            ledcWrite(PWM_CH_BUZZER, 0);
        }
    }
    portEXIT_CRITICAL_ISR(&_cnt_mutex);
}

Buzzer buzzer;
