#include <Arduino.h>
#include <array>

#include "debounced_encoder.h"
#include "display.h"
#include "thermocouple.h"

#include "hw_config.h"

static void IRAM_ATTR timer_isr(void);

static hw_timer_t *timer = NULL;
static portMUX_TYPE tm_mutex = portMUX_INITIALIZER_UNLOCKED;

static volatile int tick_count = 0;
static int buzzer_count = 0;

struct pwmLed {
    pwmLed(uint8_t pin)
    : _pin(pin), _pwm_ch(_pwm_ch_cnt++) {}
    void init() {
        ledcSetup(_pwm_ch, 4000, 8);
        ledcAttachPin(_pin, _pwm_ch);
        ledcWrite(_pwm_ch, 0);
    }
    void setPwm(int32_t duty) {
        if (duty < 0) {
            ledcWrite(_pwm_ch, 0);
        } else if (duty < 256) {
            ledcWrite(_pwm_ch, duty);
        } else if (duty < 512) {
            ledcWrite(_pwm_ch, 512 - duty);
        } else {
            ledcWrite(_pwm_ch, 0);
        }
    }
    static uint8_t _pwm_ch_cnt;
    const uint8_t _pin;
    uint8_t _pwm_ch;
};

uint8_t pwmLed::_pwm_ch_cnt = 1;

std::array<pwmLed,4> leds = { LED_TEST1, LED_TEST2, LED_TEST3, LED_TEST4 };

static void buzzer(unsigned int num_ticks)
{
    digitalWrite(LED_INT, true);
    ledcWrite(PWM_CH_BUZZER, 16);
    buzzer_count = num_ticks;
}

void setup()
{
    encoder_init();
    pinMode(LED_INT, OUTPUT);
    
    Serial.begin(115200);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timer_isr, true);
    timerAlarmWrite(timer, 250, true);

    ledcSetup(PWM_CH_BUZZER, 4000, 8);
    ledcAttachPin(BUZZER, PWM_CH_BUZZER);

    delay(100);
    display_init();

    // why does D14 PWM not work, if it's initialized before the display?!?
    for (auto l : leds) {
        l.init();
    }

    timerAlarmEnable(timer);
}

static bool check_encoder()
{
    bool update = false;

    static int old_encoder_pos = 0;
    int encoder_pos = encoder_position();
    if (encoder_pos != old_encoder_pos) {
        buzzer(25);
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

static void control_buzzer()
{
    if (buzzer_count > 0) {
        if (!--buzzer_count) {
            digitalWrite(LED_INT, false);
            ledcWrite(PWM_CH_BUZZER, 0);
        }
    }
}

static void IRAM_ATTR timer_isr(void)
{
    portENTER_CRITICAL_ISR(&tm_mutex);
    tick_count++;
    portEXIT_CRITICAL_ISR(&tm_mutex);

    encoder_check_rotation();
    encoder_check_switch();
    control_buzzer();

    static int k = -128;
    static int d = 1;
    for (int i = 0; i < 4; i++) {
        leds[i].setPwm(k / 8 - i * 256);
    }
    k += d;
    if (k == 256 * 5 * 8) d = -1;
    if (k == -256) d = 1;
}
