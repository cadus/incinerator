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

#include "start_screen.h"
#include "home_screen.h"
#include "time_entry_screen.h"

StartScreen::StartScreen()
: InteractiveScreen()
{
}

void StartScreen::reset()
{
    static ScreenChangeButton ok(*this, "OK", &timeEntryScreen, 100, _ys + _dy * 8, 100, _dy, "Start incineration");
    static ScreenChangeButton abort(*this, "Abort", &homeScreen, 220, _ys + _dy * 8, 100, _dy, "Abort incineration");

    _items.clear();
    _items.push_back(&ok);
    _items.push_back(&abort);

    InteractiveScreen::reset();
}

void StartScreen::draw()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t w = GxEPD2_420::WIDTH-x;
    uint16_t y = _ys + 15;
    constexpr uint16_t dy = 26;

    PrintFlags f;
    f.set(PrintFlag::justifyLeft);
    f.set(PrintFlag::bigFont);

    print("1. Place waste into combustion", x, y, w, dy, f);
    print("    chamber", x, y += dy, w, dy, f);

    print("2. Connect gas hoses", x, y += dy+12, w, dy, f);

    print("3. Check availability of propane", x, y += dy+12, w, dy, f);
    print("    and electricity", x, y += dy, w, dy, f);

    InteractiveScreen::draw();
}

StartScreen startScreen;
