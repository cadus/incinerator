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

#include "home_screen.h"
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

BchTest::BchTest(InteractiveScreen& parent, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                 BurnChamber& burnChamber)
: TestButton(parent, "Burner", x, y, w, h,
             "Toggle " + burnChamber.getName() + " burner")
, _burnChamber(burnChamber)
{
}

void BchTest::toggle()
{
    auto m = _burnChamber.getMode();
    if (m == BurnChamber::mode::idle) {
        _burnChamber.start();
    } else {
        _burnChamber.reset();
    }
}

std::string BchTest::getState()
{
    return _burnChamber.getModeStr();
}

IgnTest::IgnTest(InteractiveScreen& parent, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                 Ignition& ignition)
: TestButton(parent, "Ignition", x, y, w, h,
             "Toggle " + ignition.getName() + " Ignition")
, _ignition(ignition)
{
}

void IgnTest::toggle()
{
    auto m = _ignition.getMode();
    if (m == Ignition::mode::idle) {
        _ignition.start();
    } else {
        _ignition.reset();
    }
}

std::string IgnTest::getState()
{
    return _ignition.getModeStr();
}

IncineratorTest::IncineratorTest(InteractiveScreen& parent,
                                 uint16_t x, uint16_t y, uint16_t w, uint16_t h)
: TestButton(parent, "Incinerator", x, y, w, h, "Toggle Incinerator")
{
}

void IncineratorTest::toggle()
{
    auto m = incinerator.getMode();
    if (m == Incinerator::mode::idle) {
        incinerator.start();
    } else {
        incinerator.reset();
    }
}

std::string IncineratorTest::getState()
{
    return incinerator.getModeStr();
}

AirPumpTest::AirPumpTest(InteractiveScreen& parent,
                         uint16_t x, uint16_t y, uint16_t w, uint16_t h)
: TestButton(parent, "AirPump", x, y, w, h, "Toggle Air Pump")
{
}

void AirPumpTest::toggle()
{
    if (incinerator.airPump.isOn()) {
        incinerator.airPump.off();
    } else {
        incinerator.airPump.on();
    }
}

std::string AirPumpTest::getState()
{
    return incinerator.airPump.isOn() ? "ON" : "OFF";
}

ValveTest::ValveTest(InteractiveScreen& parent,
           uint16_t x, uint16_t y, uint16_t w, uint16_t h,
           std::string name,
           BurnChamber& bch, bool highLevel)
: TestButton(parent, name, x, y, w, h, "Toggle " + name)
, _bch(bch)
, _highLevel(highLevel)
, _active(false)
{
}

void ValveTest::toggle()
{
    ValveState state = _bch.getValveState();
    const ValveState onState = (_highLevel ? ValveState::high : ValveState::low);
    bool active = (state == onState);
    // toggle
    _bch.setValveState(active ? ValveState::off : onState);
}

std::string ValveTest::getState()
{
    ValveState state = _bch.getValveState();
    const ValveState onState = (_highLevel ? ValveState::high : ValveState::low);

    return state == onState ? "ON" : "OFF";
}

TestScreen::TestScreen()
: InteractiveScreen(true)
{
}

void TestScreen::reset()
{
    int16_t x = _xs, w = _dx;
    int16_t y = _ys;

    static IncineratorTest incTest(*this, 0, y + _dy / 2, x + _dx, _dy);
    y += _dy * 2;
 
    static BchTest bchMainTest(*this, x, y + _dy / 2, w, _dy, incinerator.burnerMain);
    static BchTest bchAftTest(*this, x, y + (_dy * 5) / 2, w, _dy, incinerator.burnerAft);

    x += _dx;
    static IgnTest ignMainTest(*this, x, y + _dy * 0, w, _dy, incinerator.burnerMain.ignition);
    static ValveTest valveMainTest(*this, x, y + _dy * 1, w, _dy, "VLV MAIN", incinerator.burnerMain);

    static IgnTest ignAftTest(*this, x, y + _dy * 2, w, _dy, incinerator.burnerAft.ignition);
    static ValveTest valveAftHiTest(*this, x, y + _dy * 3, w, _dy, "VLV AFT", incinerator.burnerAft);

    static AirPumpTest airPumpTest(*this, x, y + _dy * 4, w, _dy);

    static ScreenChangeButton exit(*this, "Exit", &homeScreen, 50, _ys + _dy * 8, 100, _dy, "Exit test screen");

    _items.clear();
    _items.push_back(&incTest);
    _items.push_back(&bchMainTest);
    _items.push_back(&bchAftTest);
    _items.push_back(&ignMainTest);
    _items.push_back(&valveMainTest);
    _items.push_back(&ignAftTest);
    _items.push_back(&valveAftHiTest);
    _items.push_back(&airPumpTest);
    _items.push_back(&exit);

    InteractiveScreen::reset();
}

void TestScreen::draw()
{
    InteractiveScreen::draw();

    PrintFlags flags;
    flags.set(PrintFlag::bold);

    int16_t y = _ys + _dy * 2;

    _d.drawFastHLine(0, y, _d.width(), GxEPD_BLACK);
    print("MAIN", 0, y + _dy / 2, _xs, _dy, flags);
    _d.drawFastHLine(0, y + _dy * 2, _d.width(), GxEPD_BLACK);
    print("AFT", 0, y + (_dy * 5) / 2, _xs, _dy, flags);
    _d.drawFastHLine(0, y + _dy * 4, _d.width(), GxEPD_BLACK);
}

TestScreen testScreen;
