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

Ignition::Ignition(std::string name, uint8_t pin, Thermocouple& thermocouple)
: _name(name)
, _pin(pin)
, _startFlag(false)
, _thermocouple(thermocouple)
{
}

void Ignition::init()
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

Ignition::mode Ignition::getMode()
{
    return _mode;
}

void Ignition::start()
{
    _startFlag = true;
}

void Ignition::reset()
{
    _resetFlag = true;
}

bool Ignition::ready()
{
    return getMode() == mode::burning;
}

bool Ignition::failed()
{
    return getMode() == mode::failure;
}

void Ignition::task()
{
    switch (_mode) {
    default:
    case mode::idle:
        if (!_startFlag) {
            break;
        }
        _startFlag = false;
        _mode = mode::start_set;
        _retries = _num_retries;
        // fall through
    case mode::start_set:
        digitalWrite(_pin, HIGH);
        _timeout.set(_pulse_ms);
        _mode = mode::start_wait;
        break;
    case mode::start_wait:
        if (!_timeout.elapsed()) {
            break;
        }
        _mode = mode::start_reset;
        // fall through
    case mode::start_reset:
        digitalWrite(_pin, LOW);
        _timeout.set(_check_delay_ms);
        _temp_begin = _thermocouple.get().external;
        _mode = mode::check_wait;
        break;
    case mode::check_wait:
        if (!_timeout.elapsed()) {
            break;
        }
        _mode = mode::check;
        // fall through
    case mode::check: {
        const float temp_end = _thermocouple.get().external;
        if (temp_end - _temp_begin < _check_delta_T) {
            // Temperature has not risen enough - assuming the ignition didn't work
            _retries--;
            if (!_retries) {
                _mode = mode::failure;
            } else {
                _mode = mode::start_set;
            }
        } else {
            // Temperature has risen enough. We're burning!
            _mode = mode::burning;
        }
    } break;
    case mode::burning:
    case mode::failure:
        break;
    case mode::stop:
        if (_resetFlag) {
            digitalWrite(_pin, LOW);
            _mode = mode::idle;
        }
        break;
    }
}
