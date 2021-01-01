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

#include "screen.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include "hw_config.h"
#include "incinerator/incinerator.h"
#include "ui/icons.h"
#include "util/syslog.h"

extern void GxEPD2_busyWaitCallback();

GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> Screen::_d(GxEPD2_420(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
float Screen::_progressPercent = 0.f;
bool Screen::_snowflakeMode = false;
std::string Screen::_statusStr = "";

Screen::Screen()
: _nextScreen(nullptr)
{
}

void Screen::init()
{
    // init display
    _d.init();
}

void Screen::setProgress(float percent)
{
    _progressPercent = percent;
}

void Screen::setSnowflake(bool enable)
{
    _snowflakeMode = enable;
}

void Screen::setStatus(const std::string s)
{
    _statusStr = s;
}

void Screen::setNextScreen(Screen* nextScreen)
{
    _nextScreen = nextScreen;
}

Screen* Screen::getNextScreen()
{
    return _nextScreen;
}

void Screen::print(const std::string s, uint16_t x, uint16_t y, uint16_t w, uint16_t h, PrintFlags flags)
{
    constexpr uint16_t margin = 4;
    if (flags[PrintFlag::drawRect]) {
        const uint16_t extra_margin = flags[PrintFlag::bigFont] ? 4 : 0;
        _d.drawRect(x - margin, y - extra_margin, w + margin * 2, h + extra_margin, GxEPD_BLACK);
    }
    if (flags[PrintFlag::invert]) {
        _d.fillRect(x - margin, y, w + margin * 2, h, GxEPD_BLACK);
        _d.setTextColor(GxEPD_WHITE);
        _d.setFont(&FreeSansBold9pt7b);
    } else if (flags[PrintFlag::bigFont]) {
        _d.setFont(&FreeSans12pt7b);
    } else {
        _d.setTextColor(GxEPD_BLACK);
        _d.setFont(flags[PrintFlag::bold] ? &FreeSansBold9pt7b : &FreeSans9pt7b);
    }
    int16_t x1, y1;
    uint16_t wT, hT;
    _d.getTextBounds(s.c_str(), 0, 0, &x1, &y1, &wT, &hT);
    if (flags[PrintFlag::justifyLeft]) {
        _d.setCursor(x - x1,
                     y + 2 * h / 3 + 1);
    } else if (flags[PrintFlag::justifyRight]) {
        _d.setCursor(x + (w - wT) - x1,
                     y + 2 * h / 3 + 1);
    } else {
        _d.setCursor(x + (w - wT) / 2 - x1,
                     y + 2 * h / 3 + 1);
    }
    _d.print(s.c_str());
}

void Screen::update(bool fullRefresh)
{
    unsigned long start = millis();

    constexpr uint16_t top_bar_height = 40;
    constexpr uint16_t bottom_bar_height = 32;
    constexpr uint16_t line_margin = 3;

    constexpr uint16_t icon_box_width = 36;
    constexpr uint16_t temp_box_width = 64;
    constexpr uint16_t icon_prog_width = 16;

    // Init screen drawing
    _d.setRotation(0);
    _d.setPartialWindow(0, 0, _d.width(), _d.height());
    _d.fillScreen(GxEPD_WHITE);

    // Draw top bar. Start at the left side
    uint16_t x = 0;
    // Draw thermometer icon
    icon_thermometer.draw(_d, x, 0, icon_box_width, top_bar_height);
    x += icon_box_width;

    // Print burn chamber temperatures
    for (int i = 0; i < 2; i++) {
        PrintFlags flags;
        flags.set(PrintFlag::bold);
        char tmp[16] = { 0 };
        BurnChamber& b = i ? incinerator.burnerAft : incinerator.burnerMain;
        snprintf(tmp, sizeof(tmp), "%d", int(b.thermocouple.get().external));
        print(tmp, x, 0, temp_box_width, top_bar_height, flags);

        x += temp_box_width;
        _d.drawFastVLine(x, line_margin, top_bar_height - (line_margin * 2), GxEPD_BLACK);
    }

    // Draw 2nd line after temperature readouts
    x += line_margin;
    _d.drawFastVLine(x, line_margin, top_bar_height - (line_margin * 2), GxEPD_BLACK);
    x += 2;

    // Draw stopwatch icon
    icon_clock.draw(_d, x, 0, icon_box_width, top_bar_height);
    x += icon_box_width;

    // Draw progress bar
    for (int i = 0; i < 10; i++) {
        bool filled = _progressPercent >= 10 * i + 5;
        const Icon& icon = filled ? icon_box_filled : icon_box_clear;
        icon.draw(_d, x, 0, icon_prog_width, top_bar_height);
        x += icon_prog_width;
    }

    // Draw flame (or snowflake) to the right
    const Icon& icon = _snowflakeMode ? icon_snowflake : icon_flame;
    icon.draw(_d, x, 0, icon_box_width, top_bar_height);

    // Draw line below top bar
    _d.drawFastHLine(line_margin, top_bar_height, _d.width() - (line_margin * 2), GxEPD_BLACK);

    // Draw line on top of bottom bar
    _d.drawFastHLine(line_margin, _d.height() - bottom_bar_height, _d.width() - (line_margin * 2), GxEPD_BLACK);

    // Let the subclass screen do its thing
    _d.setFont(&FreeSans9pt7b);
    draw();

    // Print status string on bottom bar
    print(_statusStr, 0, _d.height() - bottom_bar_height, _d.width(), bottom_bar_height);

    // partial update or full update if necessary
    _d.display(!fullRefresh);

    if (0) {
        unsigned long elapsed = millis() - start;
        syslog(LOG_DEBUG, "time spent: %lu ms", elapsed);
    }
}
