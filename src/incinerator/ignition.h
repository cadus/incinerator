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

#pragma once

#include "thermocouple.h"
#include "util/timeout.h"
#include <stdint.h>
#include <string>

#include "macros.h"

class Ignition
{
    NOT_COPYABLE(Ignition);
    NOT_MOVEABLE(Ignition);

public:
    enum mode {
        idle, // Idle / off
        start_set, // Set ignition pulse
        start_wait, // Wait until ignition period is done
        start_reset, // Reset ignition pulse
        check_wait, // Wait for temperature rise
        check, // Check temperature rise
        burning, // Temperature check OK, Burning
        failure, // Temperature check failed repeatedly
        stop, // Stop ignition & reset state
    };

    Ignition(std::string name, uint8_t pin, Thermocouple& thermocouple);
    mode getMode();
    std::string getModeStr();
    std::string getName();
    void init();
    void start();
    void reset();
    bool ready();
    bool failed();
    void task();

private:
    const std::string _name;
    const uint8_t _pin;
    mode _mode;
    bool _startFlag;
    bool _resetFlag;
    Timeout _timeout;

    static constexpr uint32_t _pulse_ms = 100;
    static constexpr uint8_t _num_retries = 5;

    float _temp_begin;
    uint8_t _retries;
    static constexpr uint32_t _check_delay_ms = 5000;
    static constexpr float _check_delta_T = 10.f;

    Thermocouple& _thermocouple;
};
