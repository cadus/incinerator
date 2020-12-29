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

#include "incinerator.h"

#include <map>

#include "hw_config.h"
#include "util/sys_config.h"
#include "util/syslog.h"
#include "ui/screens/screen.h"

Incinerator::Incinerator()
: burnerMain("MAIN", IGNITION_MAIN, TEMP_MAIN_CS, VALVE_MAIN)
, burnerAft("AFT", IGNITION_AFT, TEMP_AFT_CS, VALVE_AFT_HI, VALVE_AFT_LO)
, airPump(AIRPMP, PWM_CH_AIRPMP)
{
}

void Incinerator::init()
{
    burnerMain.init();
    burnerAft.init();
    airPump.init();
}

void Incinerator::task()
{
    burnerMain.task();
    burnerAft.task();
    fsm();
}

Incinerator::mode Incinerator::getMode() const
{
    return _mode;
}

std::string Incinerator::getModeStr() const
{
    static const std::map<mode, std::string> lookupTbl {
        {idle, "Idle"},
        {startAft, "StartAft"},
        {waitAft, "WaitAft"},
        {startMain, "StartMain"},
        {waitMain, "WaitMain"},
        {burnActive, "BurnActive"},
        {coolDown, "CoolDown"},
        {finished, "Finished"},
        {failed, "Failed"}
    };
    auto it = lookupTbl.find(getMode());
    return it != lookupTbl.end() ? it->second : "N/A";
}

void Incinerator::start()
{
    _startFlag = true;
}

void Incinerator::reset()
{
    burnerMain.reset();
    burnerAft.reset();
    airPump.off();
    _mode = mode::idle;
}

void Incinerator::fail()
{
    reset();
    _mode = mode::failed;
}

std::string Incinerator::fmt_minutes(uint32_t seconds) const
{
    char tmp[100];
    snprintf(tmp, sizeof(tmp), "%d:%02d", seconds / 60, seconds % 60);
    return std::string(tmp);
}


void Incinerator::fsm()
{
    float percent;

    switch (_mode) {
    default:
    case mode::idle:
        if (!_startFlag) {
            break;
        }
        _startFlag = false;
        _mode = mode::startAft;
        // fall through
    case mode::startAft:
        syslog(LOG_INFO, "Starting afterburner");
        burnerAft.start();
        _mode = mode::waitAft;
        break;
    case mode::waitAft:
        if (burnerAft.getMode() == BurnChamber::mode::failed) {
            syslog(LOG_INFO, "Afterburner failed");
            fail();
            break;
        }
        if (!burnerAft.isBurning()) {
            break;
        }
        _mode = mode::startMain;
        // fall through
    case mode::startMain:
        syslog(LOG_INFO, "Starting main burner");
        burnerMain.start();
        airPump.on();
        _mode = mode::waitMain;
        // fall through
    case mode::waitMain:
        if (burnerMain.getMode() == BurnChamber::mode::failed
            || burnerAft.getMode() == BurnChamber::mode::failed) {
            syslog(LOG_INFO, "Burner failed");
            fail();
            break;
        }
        if (!burnerMain.isBurning()) {
            break;
        }
        _mode = mode::burnActive;
        _burn_seconds = sysconfig.get("burn_time") * 60;
        _burn_seconds_elapsed = 0;
        syslog(LOG_INFO, "Burning in progress");
        _timeout.set(1000);
        // fall through
    case mode::burnActive:
        if (burnerMain.getMode() == BurnChamber::mode::failed
            || burnerAft.getMode() == BurnChamber::mode::failed) {
            syslog(LOG_INFO, "Burner failed");
            fail();
            break;
        }
        if (!_timeout.elapsed()) {
            break;
        }
        _burn_seconds_elapsed++;
        _timeout.set(1000);
        percent = (100.f * (float)_burn_seconds_elapsed) / (float)_burn_seconds;
        syslog(LOG_INFO, "Burning %s / %s (%d%%)",
                         fmt_minutes(_burn_seconds_elapsed).c_str(),
                         fmt_minutes(_burn_seconds).c_str(),
                         (int)percent);
        Screen::setProgress(percent);
        if (_burn_seconds_elapsed < _burn_seconds) {
            break;
        }
        // fall through
        reset();
        _mode = mode::coolDown;
        syslog(LOG_INFO, "Burn finished, cooling down");
    case mode::coolDown:
        // TODO measure temp. & check if both chambers cooled down enough
        break;
    case mode::finished:
    case mode::failed:
        break;
    }
}

Incinerator incinerator;
