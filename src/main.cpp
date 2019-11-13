#include <Arduino.h>
#include <TimerOne.h>

#include "hw_config.h"

static void encoder_switch_isr(void);
static void timer_isr(void);

void setup()
{
    pinMode(ROTARY_SW, INPUT_PULLUP);
    pinMode(ROTARY_CLK, INPUT_PULLUP);
    pinMode(ROTARY_DT, INPUT_PULLUP);

    pinMode(TESTPIN1, OUTPUT);
    pinMode(TESTPIN2, OUTPUT); 

    Timer1.initialize(250);
    Timer1.attachInterrupt(timer_isr);

    pinMode(LED_BUILTIN, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(ROTARY_SW), encoder_switch_isr, FALLING);
    Serial.begin(115200);
}

volatile bool flag = false;

void loop()
{
    if (flag) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        flag = false;
    }
}

static void encoder_switch_isr(void)
{
    int counter = 0;
    for(int i = 0; i < 20; i++) {
        if(digitalRead(ROTARY_SW) == 0) {
            counter++;
        } else {
            counter = 0;
        }
        delay(4);
    }
    if(counter >= 5) {
        flag = true;
    }
}

#define DEBOUNCE_NUM_CYCLES 10
#define DEBOUNCE_MASK ((1<<DEBOUNCE_NUM_CYCLES)-1)

class DebouncedPin {
public:
    DebouncedPin(uint8_t pin)
    :_pin(pin),_history(0) {};

    void update()
    {
        _history <<= 1;
        if (digitalRead(_pin)) {
            _history |= 1;
        }
        _history &= DEBOUNCE_MASK;
    };

    bool stable()
    {
        return _history == DEBOUNCE_MASK || _history == 0;
    }

    bool level()
    {
        return _history != 0;
    }

private:
    uint8_t _pin;
    uint16_t _history;
};

static void timer_isr(void)
{
//    digitalWrite(TESTPIN2, HIGH);

    static DebouncedPin clk(ROTARY_CLK), dt(ROTARY_DT);
    dt.update();
    clk.update();
    if (dt.stable()) {
        digitalWrite(TESTPIN2, dt.level() ? HIGH : LOW);
    }
    if (clk.stable()) {
        digitalWrite(TESTPIN1, clk.level() ? HIGH : LOW);
    }

//    digitalWrite(TESTPIN2, LOW);
}