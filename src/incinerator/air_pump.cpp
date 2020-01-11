/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * This file is part of CADUS Incinerator, https://github.com/patrislav1/incinerator
 * Copyright (C) 2019-2020 Patrick Huesmann <info@patrick-huesmann.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "air_pump.h"

#include "hw_config.h"
#include "util/sys_config.h"
#include <Arduino.h>

AirPump::AirPump(uint8_t pin, uint8_t pwm_ch)
: _pin(pin)
, _pwm_ch(pwm_ch)
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
    dutyCycle *= 1 << 10;
    dutyCycle /= 100;
    ledcWrite(_pwm_ch, dutyCycle);
}

void AirPump::off()
{
    ledcWrite(_pwm_ch, 0);
}

bool AirPump::isOn()
{
    return ledcRead(_pwm_ch) != 0;
}
