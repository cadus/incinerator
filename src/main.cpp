#include <Arduino.h>

#include "buzzer.h"
#include "debounced_encoder.h"
#include "display.h"
#include "thermocouple.h"

#include "hw_config.h"

static void IRAM_ATTR timer_isr(void);

static hw_timer_t *timer = NULL;
static portMUX_TYPE tm_mutex = portMUX_INITIALIZER_UNLOCKED;

static volatile int tick_count = 0;

void setup()
{
    encoder_init();
    pinMode(LED_INT, OUTPUT);
    
    Serial.begin(115200);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timer_isr, true);
    timerAlarmWrite(timer, 250, true);

    buzzer.init();

    delay(100);
    display_init();

    timerAlarmEnable(timer);
}

static bool check_encoder()
{
    bool update = false;

    static int old_encoder_pos = 0;
    int encoder_pos = encoder_position();
    if (encoder_pos != old_encoder_pos) {
        buzzer.buzz(25);
        old_encoder_pos = encoder_pos;
        Serial.print(encoder_pos, DEC);
        Serial.println();
        update |= true;
    }

    if (encoder_switch()) {
        Serial.println("SW pressed.");
        update |= true;
    }

    return update;
}

void GxEPD2_busyWaitCallback()
{
    check_encoder();
}

void loop()
{
    if (check_encoder() || (tick_count >= 1000 * 4)) {
        tick_count = 0;
        display_redraw();
    }
}

static void IRAM_ATTR timer_isr(void)
{
    portENTER_CRITICAL_ISR(&tm_mutex);
    tick_count++;
    portEXIT_CRITICAL_ISR(&tm_mutex);

    encoder_check_rotation();
    encoder_check_switch();
    buzzer.task();
}
