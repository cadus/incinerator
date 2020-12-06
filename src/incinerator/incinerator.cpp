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

#include "incinerator.h"

#include "hw_config.h"

#include "util/syslog.h"

Incinerator::Incinerator()
: _burner_main("MAIN", IGNITION_MAIN, TEMP_MAIN_CS)
, _burner_aft("AFT", IGNITION_AFT, TEMP_AFT_CS)
, _airPump(AIRPMP, PWM_CH_AIRPMP)
{
}

void Incinerator::init()
{
    _burner_main.init();
    _burner_aft.init();
    _airPump.init();
}

void Incinerator::task()
{
    _burner_main.task();
    _burner_aft.task();
}

thermocouple_meas_t Incinerator::getTemp(burn_chamber_t ch)
{
    switch (ch) {
    default:
        syslog(LOG_ERROR, "Unknown burn chamber %d", ch);
        return (thermocouple_meas_t){ 0, 0 };
    case chamber_main:
        return _burner_main.thermocouple.get();
    case chamber_aft:
        return _burner_aft.thermocouple.get();
    }
}

Incinerator incinerator;
