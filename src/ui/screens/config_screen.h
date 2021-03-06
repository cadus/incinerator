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
#include <string>
#include <vector>

class ConfigItem : public ValueEntry
{
public:
    ConfigItem(InteractiveScreen& parent,
               std::string helpText,
               std::string id,
               std::string desc,
               std::string unit,
               int step,
               int lowerBound,
               int upperBound,
               uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    virtual void update() override;

    std::string _helpText;
    std::string _id;
};

class ConfigScreen : public InteractiveScreen
{
public:
    ConfigScreen();

    void reset() override;
    void draw() override;

private:
    static constexpr uint16_t _dx = 160;
    static constexpr uint16_t _xs = 400 - _dx * 2;
    static constexpr uint16_t _ys = _content_y;
    static constexpr uint16_t _dy = 24;
};

extern ConfigScreen configScreen;
