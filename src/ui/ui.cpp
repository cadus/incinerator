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

#include "ui.h"
#include "buzzer.h"
#include "debounced_encoder.h"
#include "screens/conf_screen.h"

Ui::Ui()
: _current(nullptr)
, _screenChange(true)
, _updateReq(false)
, _encoderPos(0)
, _encoderSw(false)
{
}

void Ui::init()
{
    Screen::init();
    buzzer.init();
    encoder_init();

    _current = &confScreen;
    _current->reset();
}

void Ui::task()
{
    if (_to.elapsed() || _updateReq) {
        _updateReq = false;
        _to.set(1000);
        bool fullRefresh = _screenChange;
        // reset _screenChange before updating, b/c it can be set again during the update
        _screenChange = false;
        _current->update(fullRefresh);
    }
}

void Ui::backgroundTask()
{
    if (!_current || _screenChange) {
        return;
    }

    int encoderPos = encoder_position();
    if (encoderPos != _encoderPos) {
        buzzer.buzz(16);
        _updateReq |= _current->handleEncoderRotation(encoderPos - _encoderPos);
        _encoderPos = encoderPos;
    }

    const bool encoderSw = encoder_switch();
    if (encoderSw != _encoderSw) {
        if (encoderSw) {
            buzzer.buzz(50);
            _updateReq |= _current->handleEncoderSwitch();
        }
        _encoderSw = encoderSw;
    }
    auto next = _current->getNextScreen();
    if (next) {
        // Screen has changed
        _current->setNextScreen(nullptr);
        _current = next;
        _current->reset();
        _updateReq |= true;
        _screenChange = true;
    }
}
