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

#include "confirmation_screen.h"
#include "burn_screen.h"
#include "time_entry_screen.h"
#include "home_screen.h"

#include "util/sys_config.h"

ConfirmationScreen::ConfirmationScreen()
: InteractiveScreen()
{
}

void ConfirmationScreen::reset()
{
    static ScreenChangeButton start(*this, "Start", &burnScreen, 70, _ys + _dy * 8, 80, _dy, "Start incineration");
    static ScreenChangeButton back(*this, "Back", &timeEntryScreen, 160, _ys + _dy * 8, 80, _dy, "Re-enter burn time");
    static ScreenChangeButton cancel(*this, "Cancel", &homeScreen, 250, _ys + _dy * 8, 80, _dy, "Abort incineration");

    _items.clear();
    _items.push_back(&start);
    _items.push_back(&back);
    _items.push_back(&cancel);

    InteractiveScreen::reset();
}

void ConfirmationScreen::draw()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t w = TFT_WIDTH-x;
    constexpr uint16_t dy = 26;
    uint16_t y = _ys + dy;

    PrintFlags f;
    f.set(PrintFlag::bigFont);

    char tmp[100];
    snprintf(tmp, sizeof(tmp), "Burn time: %d minutes", sysconfig.get("burn_time"));
    print(tmp, x, y, w, dy, f);
    print("Start burning process?", x, y += dy, w, dy, f);

    InteractiveScreen::draw();
}

ConfirmationScreen confirmationScreen;
