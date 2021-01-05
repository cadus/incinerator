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

#include "cooldown_screen.h"

#include <functional>
#include <map>

#include "incinerator/incinerator.h"
#include "ui/icons.h"
#include "ui/screens/home_screen.h"
#include "ui/screens/failure_screen.h"

CooldownScreen::CooldownScreen()
: InteractiveScreen(true)
{
}

bool CooldownScreen::finish()
{
    incinerator.reset();
    setNextScreen(&homeScreen);
    return false;
}

void CooldownScreen::reset()
{
    static PushButton okBtn(*this, "OK", std::bind(&CooldownScreen::finish, this), 150, _ys + _dy * 8, 100, _dy, "Return to home screen");
 
    _items.clear();
    if (incinerator.isFinished()) {
        _items.push_back(&okBtn);
    }

    InteractiveScreen::reset();
}

void CooldownScreen::draw()
{
    uint16_t y = _ys + 18;
    icon_snowflake_big.draw(_d, 0, y, _d.width(), _dy * 4);

    PrintFlags f;
    f.set(PrintFlag::bigFont);
    std::map<Incinerator::mode, std::string> lookup {
        { Incinerator::mode::coolDown, "Cooling down" },
        { Incinerator::mode::abortCoolDown, "Cooling down (after user abort)" },
        { Incinerator::mode::failureCoolDown, "Cooling down (after failure)" },
        { Incinerator::mode::finished, "Cool down finished" },
        { Incinerator::mode::aborted, "Cool down finished (after user abort)" },
        { Incinerator::mode::failed, "Cool down finished (after failure)" },
    };
    print(lookup[incinerator.getMode()], 0, y += _dy * 5, _d.width(), _dy, f);

    if (incinerator.isFinished()) {
        if (incinerator.getMode() == Incinerator::mode::aborted
            || incinerator.getMode() == Incinerator::mode::failed) {
            // Incineration aborted or interrupted. Show warning
            setNextScreen(&failureScreen);
            return;
        } else if (!_items.size()) {
            // Successfully finished. Refresh screen and show OK button
            setNextScreen(this);
            return;
        } else if (incinerator.getMode() == Incinerator::mode::finished) {
            print("Incineration complete.", 0, y += _dy, _d.width(), _dy, f);
        }
    }
    InteractiveScreen::draw();
}

CooldownScreen cooldownScreen;
