#include "air_pump.h"

#include <Arduino.h>
#include "hw_config.h"
#include "util/sys_config.h"

AirPump::AirPump(uint8_t pin, uint8_t pwm_ch)
:_pin(pin)
,_pwm_ch(pwm_ch)
{
}

void AirPump::init()
{
    ledcSetup(_pin, 1000, 10);
    ledcAttachPin(_pin, _pwm_ch);
}

void AirPump::on()
{
    uint32_t dutyCycle = sysconfig.get("airpmp_speed");
    // Scale 0..100% to 10 bit duty cycle
    dutyCycle *= 1<<10;
    dutyCycle /= 100;
    ledcWrite(_pwm_ch, dutyCycle);
}

void AirPump::off()
{
    ledcWrite(_pwm_ch, 0);
}
