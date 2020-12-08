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

#pragma once

#include "interactive_screen.h"
#include "incinerator/ignition.h"
#include "incinerator/burn_chamber.h"

class TestButton : public PushButton
{
public:
    TestButton(InteractiveScreen& parent,
               std::string text,
               uint16_t x, uint16_t y, uint16_t w, uint16_t h,
               std::string helpText = "");

    virtual void draw(bool selected) override;

private:
    bool handler();
    virtual void toggle() = 0;
    virtual std::string getState() = 0;
};

class IgnTest : public TestButton
{
public:
    IgnTest(InteractiveScreen& parent,
            uint16_t x, uint16_t y, uint16_t w, uint16_t h, Ignition& ignition);

private:
    virtual void toggle() override;
    virtual std::string getState() override;
    Ignition& _ignition;
};

class AirPumpTest : public TestButton
{
public:
    AirPumpTest(InteractiveScreen& parent,
                uint16_t x, uint16_t y, uint16_t w, uint16_t h);

private:
    virtual void toggle() override;
    virtual std::string getState() override;
};

class ValveTest : public TestButton
{
public:
    ValveTest(InteractiveScreen& parent,
              uint16_t x, uint16_t y, uint16_t w, uint16_t h,
              std::string name,
              BurnChamber& bch, bool highLevel);

private:
    virtual void toggle() override;
    virtual std::string getState() override;

    BurnChamber& _bch;
    bool _highLevel;
    bool _active;
};

class TestScreen : public InteractiveScreen
{
public:
    TestScreen();

    void reset() override;

private:
    static constexpr uint16_t _dx = 160;
    static constexpr uint16_t _xs = 400 - _dx * 2;
    static constexpr uint16_t _ys = _content_y;
    static constexpr uint16_t _dy = 24;
};

extern TestScreen testScreen;
