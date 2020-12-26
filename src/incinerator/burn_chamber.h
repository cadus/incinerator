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

#include <string>

#include "ignition.h"
#include "thermocouple.h"
#include "util/timeout.h"

#include "macros.h"

enum class ValveState { off, low, high };

class BurnChamber
{
    NOT_COPYABLE(BurnChamber);
    NOT_MOVEABLE(BurnChamber);

public:
    enum mode {
        idle,
        startIgnition,
        waitIgnition,
        waitTemp,
        burnHigh,
        waitBurnHigh,
        burnLow,
        failed
    };

    BurnChamber(std::string name, uint8_t ignition_pin, uint8_t thermocouple_cs, uint8_t valve_hi, uint8_t valve_lo=0);

    mode getMode() const;
    std::string getModeStr() const;
    std::string getName() const;
    void init();
    void start();
    void reset();
    void task();

    void setValveState(ValveState state);
    ValveState getValveState() const;

    Ignition ignition;
    Thermocouple thermocouple;

private:
    void fsm();
    void checkIgnition(mode destState);

    mode _mode = mode::idle;
    const std::string _name;
    Timeout _tempReadTimeout;
    uint8_t _valveHiPin;
    uint8_t _valveLoPin;
    ValveState _valveState;
    bool _startFlag = false;

    static constexpr uint32_t _tempReadInterval = 500;
};
