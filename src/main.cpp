#include <Arduino.h>
#include <TimerOne.h>

#include "debounced_encoder.h"
#include "hw_config.h"

static void timer_isr(void);

void setup()
{
    encoder_init();

    Timer1.initialize(1000);
    Timer1.attachInterrupt(timer_isr);

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    if (encoder_switch()) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
    }
    static int old_encoder_pos = 0;
    int encoder_pos = encoder_position();
    if (encoder_pos != old_encoder_pos) {
        old_encoder_pos = encoder_pos;
        Serial.print(encoder_pos, DEC);
        Serial.println();
    }
}


static void timer_isr(void)
{
    digitalWrite(TESTPIN2, HIGH);

    encoder_check_rotation();
    encoder_check_switch();

    digitalWrite(TESTPIN2, LOW);
}
