#include <Arduino.h>

#include "debounced_encoder.h"
#include "display.h"
#include "thermocouple.h"

#include "hw_config.h"

static void IRAM_ATTR timer_isr(void);

static hw_timer_t *timer = NULL;
static portMUX_TYPE tm_mutex = portMUX_INITIALIZER_UNLOCKED;

static volatile int tick_count = 0;
static int buzzer_count = 0;

static void buzzer(unsigned int num_ticks)
{
#ifdef __AVR__
    Timer1.pwm(9, 64);
#endif
    buzzer_count = num_ticks;
}

void setup()
{
    encoder_init();
    
    Serial.begin(115200);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timer_isr, true);
    timerAlarmWrite(timer, 250, true);
    timerAlarmEnable(timer);
//    delay(100);
//    display_init();
}

static void check_encoder()
{
    static int old_encoder_pos = 0;
    int encoder_pos = encoder_position();
    if (encoder_pos != old_encoder_pos) {
        buzzer(25);
        old_encoder_pos = encoder_pos;
        Serial.print(encoder_pos, DEC);
        Serial.println();
    }

    if (encoder_switch()) {
        Serial.println("SW pressed.");
        while (encoder_switch());
    }
}

void GxEPD2_busyWaitCallback()
{
    check_encoder();
}

void loop()
{
    check_encoder();
    if (tick_count >= 1000 * 4) {
        tick_count = 0;
        Serial.println("Dings");
//        display_redraw();
    }
}

static void control_buzzer()
{
#if 0
    if (buzzer_count > 0) {
        if (!--buzzer_count) {
            Timer1.pwm(9, 0);
        }
    }
#endif
}

static void IRAM_ATTR timer_isr(void)
{
    portENTER_CRITICAL_ISR(&tm_mutex);
    tick_count++;
    portEXIT_CRITICAL_ISR(&tm_mutex);

    encoder_check_rotation();
    encoder_check_switch();
    control_buzzer();

}
