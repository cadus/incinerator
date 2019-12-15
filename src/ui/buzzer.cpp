#include <Arduino.h>

#include <soc/ledc_struct.h>

#include "hw_config.h"
#include "ui/buzzer.h"
#include "util/sys_config.h"

uint32_t DRAM_ATTR Buzzer::_count = 0;
portMUX_TYPE Buzzer::_cnt_mutex = portMUX_INITIALIZER_UNLOCKED;

Buzzer::Buzzer()
{
}

void Buzzer::init()
{
    // Can't set LEDC resolution for each channel independently?!?
    ledcSetup(PWM_CH_BUZZER, 4000, 10);
    ledcAttachPin(BUZZER, PWM_CH_BUZZER);
    pinMode(LED_INT, OUTPUT);
}

void Buzzer::buzz(uint32_t buzz_length_ms)
{
    if (_count < buzz_length_ms) {
        digitalWrite(LED_INT, true);
        IRAMledcWrite(sysconfig.get("buzzer_vol") << 6);
        portENTER_CRITICAL(&_cnt_mutex);
        _count = buzz_length_ms;
        portEXIT_CRITICAL(&_cnt_mutex);
    }
}

/*
 * Duplicating some esp32-hal-ledc functionality, to have a ISR-safe ledcWrite() :-/
 */

#if CONFIG_DISABLE_HAL_LOCKS
#define LEDC_MUTEX_LOCK()
#define LEDC_MUTEX_UNLOCK()
#else
#define LEDC_MUTEX_LOCK()    do {} while (xSemaphoreTake(_ledc_sys_lock, portMAX_DELAY) != pdPASS)
#define LEDC_MUTEX_UNLOCK()  xSemaphoreGive(_ledc_sys_lock)
xSemaphoreHandle _ledc_sys_lock;
#endif
#define LEDC_CHAN(g,c) LEDC.channel_group[(g)].channel[(c)]
#define LEDC_TIMER(g,t) LEDC.timer_group[(g)].timer[(t)]

void IRAM_ATTR Buzzer::IRAMledcWrite(uint32_t duty)
{
    uint8_t group=(PWM_CH_BUZZER/8), channel=(PWM_CH_BUZZER%8);
    LEDC_MUTEX_LOCK();
    LEDC_CHAN(group, channel).duty.duty = duty << 4;//25 bit (21.4)
    if(duty) {
        LEDC_CHAN(group, channel).conf0.sig_out_en = 1;//This is the output enable control bit for channel
        LEDC_CHAN(group, channel).conf1.duty_start = 1;//When duty_num duty_cycle and duty_scale has been configured. these register won't take effect until set duty_start. this bit is automatically cleared by hardware.
        if(group) {
            LEDC_CHAN(group, channel).conf0.low_speed_update = 1;
        } else {
            LEDC_CHAN(group, channel).conf0.clk_en = 1;
        }
    } else {
        LEDC_CHAN(group, channel).conf0.sig_out_en = 0;//This is the output enable control bit for channel
        LEDC_CHAN(group, channel).conf1.duty_start = 0;//When duty_num duty_cycle and duty_scale has been configured. these register won't take effect until set duty_start. this bit is automatically cleared by hardware.
        if(group) {
            LEDC_CHAN(group, channel).conf0.low_speed_update = 1;
        } else {
            LEDC_CHAN(group, channel).conf0.clk_en = 0;
        }
    }
    LEDC_MUTEX_UNLOCK();
}

void IRAM_ATTR Buzzer::task()
{
    portENTER_CRITICAL_ISR(&_cnt_mutex);
    if (_count > 0) {
        if (!--_count) {
            digitalWrite(LED_INT, false);
            IRAMledcWrite(0);
        }
    }
    portEXIT_CRITICAL_ISR(&_cnt_mutex);
}

Buzzer buzzer;
