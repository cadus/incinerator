#include <Arduino.h>

#include "incinerator/incinerator.h"

#include "ui/buzzer.h"
#include "ui/debounced_encoder.h"
#include "ui/ui.h"

#include "util/timeout.h"
#include "util/sys_config.h"

#include "hw_config.h"

static void IRAM_ATTR timer_isr(void);
static hw_timer_t *timer = NULL;

static Ui ui;

void setup()
{    
    Serial.begin(115200);

    sysconfig.init();

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timer_isr, true);
    timerAlarmWrite(timer, 250, true);

    delay(100);
    ui.init();

    timerAlarmEnable(timer);
}

void background_task()
{
    incinerator.task();
    ui.backgroundTask();
}

void GxEPD2_busyWaitCallback()
{
    background_task();
}

void loop()
{
    ui.task();
    ui.backgroundTask();
}

static void IRAM_ATTR timer_isr(void)
{
    // every 250 us
    encoder_check_rotation();
    encoder_check_switch();

    static uint8_t count = 0;
    if (!(count++ & 3)) {
        // once per millisecond
        buzzer.task();
        Timeout::incrementMs();
    }
}
