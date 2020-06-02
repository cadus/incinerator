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

#include "test_screen.h"

#include "conf_screen.h"
#include "incinerator/incinerator.h"

TestButton::TestButton(InteractiveScreen& parent,
                       std::string text,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                       std::string helpText)
: PushButton(parent, text, std::bind(&TestButton::handler, this), x, y, w / 2, h, helpText, false)
{
}

void TestButton::draw(bool selected)
{
    PushButton::draw(selected);
    _parent.print(getState(), _x + _w, _y, _w, _h);
}

bool TestButton::handler()
{
    toggle();
    return true;
}

AirPumpTest::AirPumpTest(InteractiveScreen& parent,
                         uint16_t x, uint16_t y, uint16_t w, uint16_t h)
: TestButton(parent, "AirPump", x, y, w, h, "Toggle Air Pump")
{
}

void AirPumpTest::toggle()
{
    if (incinerator._airPump.isOn()) {
        incinerator._airPump.off();
    } else {
        incinerator._airPump.on();
    }
}

std::string AirPumpTest::getState()
{
    return incinerator._airPump.isOn() ? "ON" : "OFF";
}

TestScreen::TestScreen()
: InteractiveScreen(true)
{
}

void TestScreen::reset()
{
    static AirPumpTest airPumpTest(*this, 100, _ys + _dy * 0, 200, _dy);
    static ScreenChangeButton conf(*this, "Config", &confScreen, 100, _ys + _dy * 8, 100, _dy, "Enter config screen");
    static ScreenChangeButton exit(*this, "Exit", &confScreen, 220, _ys + _dy * 8, 100, _dy, "Exit test screen");

    _items.clear();
    _items.push_back(&airPumpTest);
    _items.push_back(&conf);
    _items.push_back(&exit);

    InteractiveScreen::reset();
}

TestScreen testScreen;
