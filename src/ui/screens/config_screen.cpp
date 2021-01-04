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

#include "config_screen.h"

#include <string>

#include "home_screen.h"
#include "util/sys_config.h"

ConfigItem::ConfigItem(InteractiveScreen& parent,
                       std::string helpText,
                       std::string id,
                       std::string desc,
                       std::string unit,
                       int step,
                       int lowerBound,
                       int upperBound,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h)
: ValueEntry(parent, desc, unit,
             sysconfig.get(id), step, lowerBound, upperBound,
             x, y, w, h, 40, helpText)
, _id(id)
{
}

void ConfigItem::update()
{
    sysconfig.set(_id, _val);
}

ConfigScreen::ConfigScreen()
: InteractiveScreen(true)
{
}

void ConfigScreen::reset()
{
    static std::vector<ConfigItem> confs = {
        { *this, "Low temp.threshold to open valve",
          "main_T_low", "Temp.Low", "C", 10, 300, 700,
          _xs + _dx * 0, _ys + _dy * 0, _dx, _dy },
        { *this, "High temp.threshold to close valve",
          "main_T_high", "Temp.High", "C", 10, 300, 700,
          _xs + _dx * 0, _ys + _dy * 1, _dx, _dy },
        { *this, "Ignition control timeout",
          "main_ign_t", "Ign.Time", "s", 1, 1, 15,
          _xs + _dx * 1, _ys + _dy * 0, _dx, _dy },
        { *this, "Ignition control temp.delta",
          "main_ign_delta", "Ign.Delta", "C", 1, 1, 100,
          _xs + _dx * 1, _ys + _dy * 1, _dx, _dy },
        { *this, "Low temp.threshold to open valve",
          "aft_T_low", "Temp.Low", "C", 10, 1000, 1500,
          _xs + _dx * 0, _ys + _dy * 2, _dx, _dy },
        { *this, "High temp.threshold to close valve",
          "aft_T_high", "Temp.High", "C", 10, 1000, 1500,
          _xs + _dx * 0, _ys + _dy * 3, _dx, _dy },
        { *this, "Ignition control timeout",
          "aft_ign_t", "Ign.Time", "s", 1, 1, 15,
          _xs + _dx * 1, _ys + _dy * 2, _dx, _dy },
        { *this, "Ignition control temp.delta",
          "aft_ign_delta", "Ign.Delta", "C", 1, 1, 100,
          _xs + _dx * 1, _ys + _dy * 3, _dx, _dy },
        { *this, "Ignition pulse width",
          "ign_pulse_w", "Pulse W.", "ms", 5, 1, 100,
          _xs + _dx * 0, _ys + _dy * 4, _dx, _dy },
        { *this, "Ignition repeat interval",
          "ign_repeat_itvl", "Rep.Intvl", "ms", 10, 100, 3000,
          _xs + _dx * 1, _ys + _dy * 4, _dx, _dy },
        { *this, "Incinerator burn duration",
          "burn_time", "Burn Time", "min", 1, 1, 60,
          _xs + _dx * 0, _ys + _dy * 5, _dx, _dy },
        { *this, "Incinerator cool down temperature",
          "cool_temp", "Cool Temp.", "C", 1, 20, 100,
          _xs + _dx * 1, _ys + _dy * 5, _dx, _dy },
        { *this, "Air pump speed",
          "airpmp_speed", "Pump Spd", "%", 1, 0, 100,
          _xs + _dx * 0, _ys + _dy * 6, _dx, _dy },
        { *this, "Buzzer volume",
          "buzzer_vol", "Buzz.Vol", "", 1, 0, 8,
          _xs + _dx * 1, _ys + _dy * 6, _dx, _dy },
    };

    static ScreenChangeButton exit(*this, "Exit", &homeScreen, 220, _ys + _dy * 8, 100, _dy, "Exit config screen");

    _items.clear();
    for (auto& c : confs) {
        _items.push_back(&c);
    }
    _items.push_back(&exit);

    InteractiveScreen::reset();
}

void ConfigScreen::draw()
{
    InteractiveScreen::draw();

    PrintFlags flags;
    flags.set(PrintFlag::bold);
    print("MAIN", 0, _ys, _xs, _dy * 2, flags);
    _d.drawFastHLine(0, _ys + _dy * 2, _d.width(), GxEPD_BLACK);
    print("AFT", 0, _ys + _dy * 2, _xs, _dy * 2, flags);
    _d.drawFastHLine(0, _ys + _dy * 4, _d.width(), GxEPD_BLACK);
    print("IGN", 0, _ys + _dy * 4, _xs, _dy, flags);
    _d.drawFastHLine(0, _ys + _dy * 5, _d.width(), GxEPD_BLACK);
    print("INC", 0, _ys + _dy * 5, _xs, _dy, flags);
    _d.drawFastHLine(0, _ys + _dy * 6, _d.width(), GxEPD_BLACK);
    print("MISC", 0, _ys + _dy * 6, _xs, _dy, flags);
}

ConfigScreen configScreen;
