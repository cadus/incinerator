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

#include "incinerator/burn_chamber.h"

BurnChamber::BurnChamber(std::string name, uint8_t ignition_pin, uint8_t thermocouple_cs)
: ignition(name, ignition_pin, thermocouple)
, thermocouple(name, thermocouple_cs)
, _name(name)
{
}

void BurnChamber::init()
{
    thermocouple.init();
    ignition.init();
}

void BurnChamber::task()
{
    if (_tempReadTimeout.elapsed()) {
        _tempReadTimeout.set(_tempReadInterval);
        thermocouple.update();
    }
    ignition.task();
}
