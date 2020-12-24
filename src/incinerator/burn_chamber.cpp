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

#include "incinerator/burn_chamber.h"

#include <map>

BurnChamber::BurnChamber(std::string name, uint8_t ignition_pin, uint8_t thermocouple_cs, uint8_t valve_hi, uint8_t valve_lo)
: ignition(name, ignition_pin, thermocouple)
, thermocouple(name, thermocouple_cs)
, _name(name)
, _valve_hi(valve_hi)
, _valve_lo(valve_lo)
, _valve_state(ValveState::off)
{
}

void BurnChamber::init()
{
    thermocouple.init();
    ignition.init();
    pinMode(_valve_hi, OUTPUT);
    digitalWrite(_valve_hi, LOW);
    if (_valve_lo) {
        pinMode(_valve_lo, OUTPUT);
        digitalWrite(_valve_lo, LOW);
    }
}

BurnChamber::mode BurnChamber::getMode() const
{
    return _mode;
}

std::string BurnChamber::getModeStr() const
{
    static const std::map<mode, std::string> lookupTbl {
        {idle, "Idle"},
        {startIgnition, "StartIgn"},
        {waitIgnition, "WaitIgn"},
        {waitTemp, "WaitTemp"},
        {burnHigh, "BurnHigh"},
        {burnLow, "BurnLow"},
        {failed, "Failed"}
    };
    auto it = lookupTbl.find(getMode());
    return it != lookupTbl.end() ? it->second : "N/A";
}

std::string BurnChamber::getName() const
{
    return _name;
}

void BurnChamber::start()
{
    _startFlag = true;
}

void BurnChamber::reset()
{
    ignition.reset();
    valve_state_set(ValveState::off);
    _mode = mode::idle;
}
    
void BurnChamber::task()
{
    if (_tempReadTimeout.elapsed()) {
        _tempReadTimeout.set(_tempReadInterval);
        thermocouple.update();
    }
    ignition.task();
}

void BurnChamber::valve_state_set(ValveState state)
{
    _valve_state = state;

    if (state == ValveState::off) {
        digitalWrite(_valve_hi, LOW);
        digitalWrite(_valve_lo, LOW);
        return;
    }

    bool highLevel = (state == ValveState::high);
    digitalWrite(_valve_hi, highLevel ? HIGH : LOW);
    if (_valve_lo) {
        digitalWrite(_valve_lo, highLevel ? LOW : HIGH);
    }
}

ValveState BurnChamber::valve_state_get() const
{
    return _valve_state;
}
