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

#include "sys_config.h"

#include <vector>

SysConfig::SysConfig()
: _default({
      { "main_T_low", 500 },
      { "main_T_high", 550 },
      { "aft_T_low", 1200 },
      { "aft_T_high", 1250 },
      { "main_ign_t", 5 },
      { "main_ign_delta", 5 },
      { "aft_ign_t", 5 },
      { "aft_ign_delta", 5 },
      { "ign_pulse_w", 100 },
      { "ign_repeat_itvl", 500 },
      { "airpmp_speed", 50 },
      { "buzzer_vol", 1 },
  })
{
}

void SysConfig::init()
{
    prefs.begin("incinerator", false);

    // Remove all preferences under the opened namespace
    //preferences.clear();

    _items = _default;
    for (auto& i : _items) {
        i.second = prefs.getInt(i.first.c_str(), i.second);
    }
}

void SysConfig::reset()
{
    prefs.clear();
    _items = _default;
}

int32_t SysConfig::get(std::string name)
{
    auto it = _items.find(name);
    if (it == _items.end()) {
        return 0;
    }
    return it->second;
}

void SysConfig::set(std::string name, int32_t value)
{
    auto it = _items.find(name);
    if (it == _items.end()) {
        return;
    }
    it->second = value;
    // Only update flash if necessary
    if (prefs.getInt(it->first.c_str()) != it->second) {
        prefs.putInt(it->first.c_str(), it->second);
    }
}

SysConfig sysconfig;
