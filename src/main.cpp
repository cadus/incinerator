#include <Arduino.h>

#include "ui/buzzer.h"
#include "ui/debounced_encoder.h"
#include "ui/display.h"

#include "util/timeout.h"

#include "hw_config.h"

static void IRAM_ATTR timer_isr(void);
static hw_timer_t *timer = NULL;

void setup()
{
    encoder_init();
    
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
        buzzer.buzz(16);
        old_encoder_pos = encoder_pos;
        Serial.print(encoder_pos, DEC);
        Serial.println();
        update |= true;
    }

    static bool encoder_was_pressed = false;
    const bool encoder_pressed = encoder_switch();
    if (encoder_pressed && !encoder_was_pressed) {
        Serial.println("SW pressed.");
        buzzer.buzz(50);
        update |= true;
    }
    encoder_was_pressed = encoder_pressed;

    return update;
}

void GxEPD2_busyWaitCallback()
{
    check_encoder();
}

void loop()
{
    static Timeout to;
    if (check_encoder() || to.elapsed()) {
        to.set(1000);
        display_redraw();
    }
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
