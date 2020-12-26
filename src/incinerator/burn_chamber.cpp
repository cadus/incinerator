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

#include "util/sys_config.h"

BurnChamber::BurnChamber(std::string name, uint8_t ignition_pin, uint8_t thermocouple_cs, uint8_t valve_hi, uint8_t valve_lo)
: ignition(name, ignition_pin, thermocouple)
, thermocouple(name, thermocouple_cs)
, _name(name)
, _valveHiPin(valve_hi)
, _valveLoPin(valve_lo)
, _valveState(ValveState::off)
{
}

void BurnChamber::init()
{
    thermocouple.init();
    ignition.init();
    pinMode(_valveHiPin, OUTPUT);
    digitalWrite(_valveHiPin, LOW);
    if (_valveLoPin) {
        pinMode(_valveLoPin, OUTPUT);
        digitalWrite(_valveLoPin, LOW);
    }
}

BurnChamber::mode BurnChamber::getMode() const
{
    return _mode;
}

bool BurnChamber::isBurning() const
{
    const auto m = getMode();
    return m == waitBurnHigh || m == burnHigh || m == burnLow;
}

std::string BurnChamber::getModeStr() const
{
    static const std::map<mode, std::string> lookupTbl {
        {idle, "Idle"},
        {startIgnition, "StartIgn"},
        {waitIgnition, "WaitIgn"},
        {waitTemp, "WaitTmp"},
        {waitBurnHigh, "WaitBHi"},
        {burnHigh, "BurnHi"},
        {burnLow, "BurnLo"},
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
    setValveState(ValveState::off);
    _mode = mode::idle;
}

void BurnChamber::checkIgnition(mode destState)
{
    auto ignMode = ignition.getMode();
    if (ignMode == Ignition::mode::failure) {
        ignition.reset();
        setValveState(ValveState::off);
        _mode = mode::failed;
    }
    if (ignMode == Ignition::mode::success) {
        ignition.reset();
        _mode = destState;
    }
}

void BurnChamber::fsm()
{
    std::string name_lower = getName();
    transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower); 
    const float temp = thermocouple.get().external;

    switch (_mode) {
    default:
    case mode::idle:
        if (!_startFlag) {
            break;
        }
        _startFlag = false;
        _mode = mode::startIgnition;
        // fall through
    case mode::startIgnition:
        setValveState(ValveState::high);
        ignition.start();
        _mode = mode::waitIgnition;
        break;
    case mode::waitIgnition:
        checkIgnition(mode::waitTemp);
        break;
    case mode::waitTemp:
        if (temp >= sysconfig.get(name_lower + "_T_low")) {
            setValveState(ValveState::high);
            _mode = mode::burnHigh;
        }
        break;
    case mode::burnHigh:
        if (temp >= sysconfig.get(name_lower + "_T_high")) {
            setValveState(ValveState::low);
            _mode = mode::burnLow;
        }
        break;
    case mode::burnLow:
        if (temp < sysconfig.get(name_lower + "_T_low")) {
            setValveState(ValveState::high);
            ignition.start();
            _mode = mode::waitBurnHigh;
        }
        break;
    case mode::waitBurnHigh:
        checkIgnition(mode::burnHigh);
        break;
    case mode::failed:
        break;
    }
}

void BurnChamber::task()
{
    if (_tempReadTimeout.elapsed()) {
        _tempReadTimeout.set(_tempReadInterval);
        thermocouple.update();
    }
    ignition.task();
    fsm();
}

void BurnChamber::setValveState(ValveState state)
{
    _valveState = state;

    if (state == ValveState::off) {
        digitalWrite(_valveHiPin, LOW);
        digitalWrite(_valveLoPin, LOW);
        return;
    }

    bool highLevel = (state == ValveState::high);
    digitalWrite(_valveHiPin, highLevel ? HIGH : LOW);
    if (_valveLoPin) {
        digitalWrite(_valveLoPin, highLevel ? LOW : HIGH);
    }
}

ValveState BurnChamber::getValveState() const
{
    return _valveState;
}
