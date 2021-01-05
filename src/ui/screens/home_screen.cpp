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

#include "home_screen.h"

#include <string>

#include "config_screen.h"
#include "test_screen.h"
#include "start_screen.h"
#include "ui/icons.h"
#include "version.h"

#include "incinerator/incinerator.h"

HomeScreen::HomeScreen()
: InteractiveScreen(true)
{
}

void HomeScreen::reset()
{
    incinerator.reset();

    static ScreenChangeButton conf(*this, "Config", &configScreen, 70, _ys + _dy * 8, 80, _dy, "Enter config screen");
    static ScreenChangeButton test(*this, "Test", &testScreen, 160, _ys + _dy * 8, 80, _dy, "Enter test screen");
    static ScreenChangeButton start(*this, "Start", &startScreen, 250, _ys + _dy * 8, 80, _dy, "Enter start screen");

    _items.clear();
    _items.push_back(&conf);
    _items.push_back(&test);
    _items.push_back(&start);

    InteractiveScreen::reset();
}

void HomeScreen::draw()
{
    uint16_t y = _ys;
    icon_cadus_logo.draw(_d, 0, y, GxEPD2_420::WIDTH, _dy * 4);
    y += _dy * 4;

    const std::string about = "Incinerator " + std::string(gitversion);
    print(about, 0, y, GxEPD2_420::WIDTH, _dy, bold);

    InteractiveScreen::draw();
}

HomeScreen homeScreen;
