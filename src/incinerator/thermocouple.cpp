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

#include "thermocouple.h"
#include "hw_config.h"

#include "util/syslog.h"

Thermocouple::Thermocouple(std::string name, uint8_t cs)
: _name(name)
, _max31855(cs)
{
}

void Thermocouple::init()
{
    _max31855.begin();
}

thermocouple_meas_t Thermocouple::get() const
{
    return _curr_readout;
}

void Thermocouple::update()
{
    size_t retries = 5;
    bool success = false;

    thermocouple_meas_t tmp;
    while (retries-- && !success) {
        tmp = (thermocouple_meas_t){
            .internal = float(_max31855.readInternal()),
            .external = float(_max31855.readCelsius())
        };
        if (isnan(tmp.internal) || isnan(tmp.external)) {
            continue;
        }
        if (tmp.internal < -100 || tmp.internal > 300) {
            continue;
        }
        if (tmp.external < -100 || tmp.external > 3000) {
            continue;
        }
        success = true;
    }

    if (!success) {
        syslog(LOG_ERROR, "Thermocouple %s error (%.1f, %.1f)", _name.c_str(), tmp.internal, tmp.external);
        return;
    }

#ifdef ENABLE_TEMP_SIMULATION
    if (sim_temp != -1) {
        tmp.external = sim_temp;
    }
#endif

    _curr_readout = tmp;
    //  syslog(LOG_DEBUG, "R = %d, T_int: %.1f, T_ext: %.1f", retries, _curr_readout.internal, _curr_readout.external);
}
