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

#include "time_entry_screen.h"
#include "confirmation_screen.h"
#include "util/sys_config.h"
#include "ui/icons.h"

TimeEntryScreen::TimeEntryScreen()
: Screen()
{
}

void TimeEntryScreen::reset()
{
    _burn_time = int(sysconfig.get("burn_time"));
}

void TimeEntryScreen::draw()
{
    uint16_t x = 60;
    uint16_t y = _ys + 35;
    constexpr uint16_t dy = 26;

    PrintFlags f;
    f.set(PrintFlag::bigFont);
    PrintFlags b(f);
    b.set(PrintFlag::drawRect);
    PrintFlags r;
    r.set(PrintFlag::drawRect);
    r.set(PrintFlag::invert);

    print("Enter time:", x, y, 140, dy, f);
    x += 140;

    char tmp[20];
    snprintf(tmp, sizeof(tmp), "%d", int(_burn_time));
    print(tmp, x, y, 60, dy, b);
    x += 60;

    print("min", x, y, 60, dy, f);

    icon_dial.draw(_d, 80, 140);
    print("OK", 200, 180, 80, _dy, r);
}

bool TimeEntryScreen::handleEncoderRotation(int delta)
{
    int32_t time_prev = _burn_time;
    _burn_time += delta;
    _burn_time = max(0, _burn_time);
    _burn_time = min(60, _burn_time);
    return _burn_time != time_prev;
}

bool TimeEntryScreen::handleEncoderSwitch()
{
    sysconfig.set("burn_time", _burn_time);
    setNextScreen(&confirmationScreen);
    return false;
}

TimeEntryScreen timeEntryScreen;
