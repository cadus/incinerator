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

#include "failure_screen.h"

#include <map>

#include "incinerator/incinerator.h"
#include "ui/icons.h"
#include "ui/screens/home_screen.h"

FailureScreen::FailureScreen()
: InteractiveScreen(true)
{
}

void FailureScreen::reset()
{
    static ScreenChangeButton okBtn(*this, "OK", &homeScreen, 150, _ys + _dy * 8, 100, _dy, "Return to home screen");
 
    _items.clear();
    _items.push_back(&okBtn);

    InteractiveScreen::reset();
}

void FailureScreen::draw()
{
    uint16_t y = _ys;
    icon_warn_temp.draw(_d, 0, y, _d.width(), _dy * 4);

    PrintFlags f;
    f.set(PrintFlag::bigFont);
    std::map<Incinerator::mode, std::string> lookup {
        { Incinerator::mode::aborted, "Shutdown after user abort!" },
        { Incinerator::mode::failed, "Shutdown after failure!" },
    };
    print(lookup[incinerator.getMode()], 0, y += _dy * 5, _d.width(), _dy, f);
    print("Incineration aborted or interrupted.", 0, y += _dy, _d.width(), _dy, f);
    print("Beware of hazardous items!", 0, y += _dy, _d.width(), _dy, f);
    
    InteractiveScreen::draw();
}

FailureScreen failureScreen;
