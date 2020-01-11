/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * This file is part of CADUS Incinerator, https://github.com/patrislav1/incinerator
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

#include "syslog.h"

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#include "ui/screens/screen.h"

static constexpr log_level_t lvl_console = LOG_DEBUG;
static constexpr log_level_t lvl_display = LOG_INFO;

void syslog(log_level_t lvl, const char* fmt, ...)
{
    char tmp[300];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, argptr);
    va_end(argptr);

    if (lvl >= lvl_console) {
        Serial.println(tmp);
    }
    if (lvl >= lvl_display) {
        Screen::setStatus(tmp);
    }
}
