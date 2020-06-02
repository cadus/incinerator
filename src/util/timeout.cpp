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

#include "timeout.h"

volatile int32_t Timeout::_counter = 0;
portMUX_TYPE Timeout::_tm_mutex = portMUX_INITIALIZER_UNLOCKED;

Timeout::Timeout()
{
    set(0);
}

void Timeout::set(int32_t ms)
{
    _timeout = getMs() + ms;
}

bool Timeout::elapsed()
{
    return (getMs() - _timeout) >= 0;
}

void IRAM_ATTR Timeout::incrementMs()
{
    // called from interrupt
    portENTER_CRITICAL_ISR(&_tm_mutex);
    _counter++;
    portEXIT_CRITICAL_ISR(&_tm_mutex);
}

int32_t Timeout::getMs()
{
    return _counter;
}
