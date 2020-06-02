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

#include <functional>
#include <string>

#include "screen.h"

class InteractiveScreen;

class InteractiveItem
{
public:
    InteractiveItem(InteractiveScreen& parent,
                    uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                    std::string helpText = "");

    virtual void draw(bool selected) = 0;
    virtual bool rotate(int digits) = 0;
    virtual bool click() = 0;

protected:
    void showHelpText();

    InteractiveScreen& _parent;
    uint16_t _x;
    uint16_t _y;
    uint16_t _w;
    uint16_t _h;

    std::string _helpText;
};

class PushButton : public InteractiveItem
{
public:
    PushButton(InteractiveScreen& parent,
               std::string text,
               std::function<bool()> handler,
               uint16_t x, uint16_t y, uint16_t w, uint16_t h,
               std::string helpText = "",
               bool withRect = true);

    virtual void draw(bool selected) override;
    virtual bool rotate(int digits) override;
    virtual bool click() override;

private:
    std::string _text;
    std::function<bool()> _handler;
    bool _withRect;
};

class ScreenChangeButton : public PushButton
{
public:
    ScreenChangeButton(InteractiveScreen& parent,
                       std::string text,
                       Screen* target,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                       std::string helpText = "");
};

class ValueEntry : public InteractiveItem
{
public:
    ValueEntry(InteractiveScreen& parent,
               std::string text,
               std::string unit,
               int initial,
               int step,
               int lowerBound,
               int upperBound,
               uint16_t x, uint16_t y, uint16_t w, uint16_t h,
               uint16_t text_value_ratio = 50,
               std::string helpText = "");

    virtual void draw(bool selected) override;
    virtual bool rotate(int digits) override;
    virtual bool click() override;

    virtual void update() = 0;

    std::string _text;
    std::string _unit;
    int _val;
    int _step;
    int _lowerBound;
    int _upperBound;

    uint16_t _value_width;
};

class InteractiveScreen : public Screen
{
    friend class PushButton;
    friend class ScreenChangeButton;
    friend class ValueEntry;

public:
    InteractiveScreen(bool wraparound = false);

    void reset() override;
    void draw() override;
    bool handleEncoderRotation(int delta) override;
    bool handleEncoderSwitch() override;

protected:
    std::vector<InteractiveItem*> _items;
    ssize_t _selectedItem;
    bool _fixedItemSelection;
    bool _wraparound;
};
