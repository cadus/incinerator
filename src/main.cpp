#include <Arduino.h>
#include <SPI.h>

#include "TimerOne.h"
#include "Adafruit_MAX31855.h"

#include "debounced_encoder.h"
#include "hw_config.h"

static void timer_isr(void);

static Adafruit_MAX31855 thermocouple(MAX31855_CS);

static int ms_count = 0;

void setup()
{
    encoder_init();

    Timer1.initialize(1000);
    Timer1.attachInterrupt(timer_isr);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);

    Serial.begin(115200);
}

static void sed_leds(uint8_t mask)
{
    digitalWrite(LED5, mask & (1 << 0));
    digitalWrite(LED4, mask & (1 << 1));
    digitalWrite(LED3, mask & (1 << 2));
    digitalWrite(LED2, mask & (1 << 3));
    digitalWrite(LED1, mask & (1 << 4));
}

static void check_temp()
{
    Serial.print("Internal Temp = ");
    Serial.println(thermocouple.readInternal());

    #if 0
    double c = thermocouple.readCelsius();
    if (isnan(c)) {
        Serial.println("Something wrong with thermocouple!");
    } else {
        Serial.print("C = "); 
        Serial.println(c);
    }
    #endif
}

static const uint8_t masks[] = {
    0b00000,
    0b00001,
    0b00011,
    0b00111,
    0b01111,
    0b11111,
    0b11110,
    0b11100,
    0b11000,
    0b10000
};

static void check_encoder()
{
    static int old_encoder_pos = 0;
    int encoder_pos = encoder_position();
    if (encoder_pos != old_encoder_pos) {
        sed_leds(masks[encoder_pos % sizeof(masks)]);
        old_encoder_pos = encoder_pos;
        Serial.print(encoder_pos, DEC);
        Serial.println();
    }


    if (encoder_switch()) {
        Serial.println("SW pressed.");
        while (encoder_switch());
    }
}

void loop()
{
    check_encoder();
    if (ms_count >= 1000) {
        ms_count = 0;
        check_temp();
    }
}


static void timer_isr(void)
{
    encoder_check_rotation();
    encoder_check_switch();

    ms_count++;
}
