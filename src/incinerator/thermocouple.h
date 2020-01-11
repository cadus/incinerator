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

#pragma once

#include <stdint.h>
#include <string>

#include "Adafruit_MAX31855.h"

#include "macros.h"

typedef struct {
    float internal;
    float external;
} thermocouple_meas_t;

class Thermocouple
{
    NOT_COPYABLE(Thermocouple);
    NOT_MOVEABLE(Thermocouple);

public:
    Thermocouple(std::string name, uint8_t cs);
    thermocouple_meas_t get() const;
    void init();
    void update();

private:
    const std::string _name;
    Adafruit_MAX31855 _max31855;
    thermocouple_meas_t _curr_readout;
};
