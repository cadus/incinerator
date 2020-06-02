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

#include <GxEPD2_BW.h>
#include <bitset>
#include <string>

#include "macros.h"

class Screen
{
public:
    Screen();

    static void init();
    void update(bool fullRefresh);

    static void setProgress(float percent);
    static void setStatus(const std::string s);

    void setNextScreen(Screen* nextScreen);
    Screen* getNextScreen();

    virtual void reset() = 0;
    virtual void draw() = 0;
    virtual bool handleEncoderRotation(int delta) = 0;
    virtual bool handleEncoderSwitch() = 0;

    enum PrintFlag {
        bold,
        justifyLeft,
        justifyRight,
        drawRect,
        invert,
        numFlags
    };
    typedef std::bitset<PrintFlag::numFlags> PrintFlags;

    static void print(const std::string s, uint16_t x, uint16_t y, uint16_t w, uint16_t h, PrintFlags flags = 0);

protected:
    static GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> _d;

    // space reserved for content screen (subclass)
    static constexpr uint16_t _content_y = 40;
    static constexpr uint16_t _content_height = 300 - 76;

private:
    Screen* _nextScreen;
    static float _progressPercent;
    static std::string _statusStr;
};
