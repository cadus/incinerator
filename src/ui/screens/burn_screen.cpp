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

#include "burn_screen.h"

#include <functional>

#include "incinerator/incinerator.h"
#include "ui/icons.h"

BurnScreen::BurnScreen()
: InteractiveScreen(true)
{
}

bool BurnScreen::abort()
{
    incinerator.abort();
    return false;
}

void BurnScreen::reset()
{
    static PushButton abortBtn(*this, "Abort", std::bind(&BurnScreen::abort, this), 150, _ys + _dy * 8, 100, _dy, "Abort incineration");
 
    _items.clear();
    _items.push_back(&abortBtn);

    InteractiveScreen::reset();

    incinerator.start();
}

void BurnScreen::draw()
{
    uint16_t y = _ys + 18;
    icon_flame_big.draw(_d, 0, y, _d.width(), _dy * 4);

    PrintFlags f;
    f.set(PrintFlag::bigFont);
    print("Burning", 0, y += _dy * 5, _d.width(), _dy, f);

    InteractiveScreen::draw();
}

BurnScreen burnScreen;
