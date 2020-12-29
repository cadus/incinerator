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

#include "interactive_screen.h"

InteractiveItem::InteractiveItem(InteractiveScreen& parent,
                                 uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                                 std::string helpText)
: _parent(parent)
, _x(x)
, _y(y)
, _w(w)
, _h(h)
, _helpText(helpText)
{
}

void InteractiveItem::showHelpText()
{
    _parent.setStatus(_helpText);
}

PushButton::PushButton(InteractiveScreen& parent,
                       std::string text,
                       std::function<bool()> handler,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                       std::string helpText,
                       bool withRect)
: InteractiveItem(parent, x, y, w, h, helpText)
, _text(text)
, _handler(handler)
, _withRect(withRect)
{
}

void PushButton::draw(bool selected)
{
    Screen::PrintFlags flags;
    if (selected) {
        flags.set(Screen::PrintFlag::invert);
    } else {
        if (_withRect) {
            flags.set(Screen::PrintFlag::drawRect);
        }
    }

    _parent.print(_text, _x, _y, _w, _h, flags);
}

bool PushButton::rotate(int digits)
{
    // push button doesn't accept encoder input
    return false;
}

bool PushButton::click()
{
    return _handler();
}

ScreenChangeButton::ScreenChangeButton(InteractiveScreen& parent,
                                       std::string text,
                                       Screen* target,
                                       uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                                       std::string helpText)
: PushButton(parent,
             text,
             [this, target]() { _parent.setNextScreen(target); return true; },
             x, y, w, h, helpText)
{
}

ValueEntry::ValueEntry(InteractiveScreen& parent,
                       std::string text,
                       std::string unit,
                       int initial,
                       int step,
                       int lowerBound,
                       int upperBound,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                       uint16_t text_value_ratio,
                       std::string helpText)
: InteractiveItem(parent, x, y, w, h, helpText)
, _text(text)
, _unit(unit)
, _val(initial)
, _step(step)
, _lowerBound(lowerBound)
, _upperBound(upperBound)
, _value_width(w * text_value_ratio / 100)
{
}

void ValueEntry::draw(bool selected)
{
    uint16_t x = _x;

    Screen::PrintFlags flags;
    flags.set(Screen::PrintFlag::justifyRight);
    if (selected && !_parent._fixedItemSelection) {
        flags.set(Screen::PrintFlag::invert);
    }
    Screen::print(_text, x, _y, _w - _value_width - 4, _h, flags);

    x += _w - _value_width + 4;

    flags = 0;
    flags.set(Screen::PrintFlag::justifyLeft);
    if (selected && _parent._fixedItemSelection) {
        flags.set(Screen::PrintFlag::invert);
    }
    char tmp[80];
    snprintf(tmp, sizeof(tmp), "%d%s", _val, _unit.c_str());
    Screen::print(tmp, x, _y, _value_width - 4, _h, flags);
}

bool ValueEntry::rotate(int digits)
{
    auto val_before = _val;
    _val += digits * _step;
    _val = min(_upperBound, _val);
    _val = max(_lowerBound, _val);

    return val_before != _val;
}

bool ValueEntry::click()
{
    if (_parent._fixedItemSelection) {
        printf("%s update to %d\r\n", _text.c_str(), _val);
        update();
    }
    _parent._fixedItemSelection = !_parent._fixedItemSelection;
    return true;
}

InteractiveScreen::InteractiveScreen(bool wraparound)
: _wraparound(wraparound)
{
}

void InteractiveScreen::reset()
{
    _selectedItem = 0;
    _fixedItemSelection = false;
}

void InteractiveScreen::draw()
{
    ssize_t k = 0;
    for (auto i : _items) {
        bool isSelected = (k++ == _selectedItem);
        i->draw(isSelected);
        if (isSelected && _refreshHelpText) {
            i->showHelpText();
            _refreshHelpText = false;
        }
    }
}

bool InteractiveScreen::handleEncoderRotation(int delta)
{
    const ssize_t range = _items.size();
    if (!range) {
        // handle edge case of empty item list
        return false;
    }

    if (_selectedItem < 0 || _selectedItem >= range) {
        // shouldn't happen
        return false;
    }

    if (_fixedItemSelection) {
        // selected item receives encoder rotation
        return _items[_selectedItem]->rotate(delta);
    }

    // change item selection according to encoder rotation
    ssize_t selected_before = _selectedItem;
    _selectedItem += delta;
    if (_wraparound) {
        while (_selectedItem < 0) {
            _selectedItem += range;
        }
        while (_selectedItem >= range) {
            _selectedItem -= range;
        }
    } else {
        _selectedItem = max(_selectedItem, 0);
        _selectedItem = min(_selectedItem, range - 1);
    }

    bool itemChanged = _selectedItem != selected_before;
    _refreshHelpText |= itemChanged;
    return itemChanged;
}

bool InteractiveScreen::handleEncoderSwitch()
{
    const size_t range = _items.size();
    if (!range) {
        // handle edge case of empty item list
        return false;
    }

    if (_selectedItem < 0 || _selectedItem >= range) {
        // shouldn't happen
        return false;
    }

    return _items[_selectedItem]->click();
}
