/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * This file is part of CADUS Incinerator, https://github.com/cadus/incinerator
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

#include "ignition.h"

#include <Arduino.h>
#include <map>

#include "util/sys_config.h"

Ignition::Ignition(std::string name, uint8_t pin, Thermocouple& thermocouple)
: _name(name)
, _pin(pin)
, _thermocouple(thermocouple)
{
}

void Ignition::init()
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

Ignition::mode Ignition::getMode() const
{
    return _mode;
}

std::string Ignition::getModeStr() const
{
    static const std::map<mode, std::string> lookupTbl {
        {idle, "Idle"},
        {setCoil, "SetCoil"},
        {waitPulse, "WaitPulse"},
        {waitIgnition, "WaitIgnition"},
        {success, "Success"},
        {failure, "Failure"},
    };
    auto it = lookupTbl.find(getMode());
    return it != lookupTbl.end() ? it->second : "N/A";
}

std::string Ignition::getName() const
{
    return _name;
}

void Ignition::start()
{
    _startFlag = true;
}

void Ignition::reset()
{
    digitalWrite(_pin, LOW);
    _mode = mode::idle;
}

void Ignition::task()
{
    std::string name_lower = getName();
    transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower); 

    switch (_mode) {
    default:
    case mode::idle:
        if (!_startFlag) {
            break;
        }
        _startFlag = false;
        _burn_timeout.set(sysconfig.get(name_lower + "_ign_t") * 1000);
        _temp_begin = _thermocouple.get().external;
        _mode = mode::setCoil;
        // fall through
    case mode::setCoil:
        digitalWrite(_pin, HIGH);
        _pulse_timeout.set(sysconfig.get("ign_pulse_w"));
        _mode = mode::waitPulse;
        break;
    case mode::waitPulse:
        if (!_pulse_timeout.elapsed()) {
            break;
        }
        digitalWrite(_pin, LOW);
        _pulse_timeout.set(sysconfig.get("ign_repeat_itvl"));
        _mode = mode::waitIgnition;
        break;
    case mode::waitIgnition:
        {
            const float temp_end = _thermocouple.get().external;
            if (temp_end - _temp_begin >= sysconfig.get(name_lower + "_ign_delta")) {
                // Temperature has risen enough. We're burning!
                _mode = mode::success;
                break;
            }
        }
        if (_burn_timeout.elapsed()) {
            // Timeout for burn start elapsed. Ignition failure!
            _mode = mode::failure;
            break;
        }
        if (_pulse_timeout.elapsed()) {
            // Timeout for pulse interval elapsed. Try to ignite again.
            _mode = mode::setCoil;
            break;
        }
        break;
    case mode::success:
    case mode::failure:
        break;
    }
}
