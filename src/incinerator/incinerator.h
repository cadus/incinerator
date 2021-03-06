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

#include "air_pump.h"
#include "burn_chamber.h"
#include "macros.h"
#include "util/timeout.h"

class Incinerator
{
    NOT_COPYABLE(Incinerator);
    NOT_MOVEABLE(Incinerator);

public:
    enum mode {
        idle, // Idle / off
        startAft, // Start afterburner
        waitAft, // Wait for afterburner ready
        startMain, // Start main burner
        waitMain, // Wait for main burner ready
        burnActive, // Wait for burn time elapsed
        coolDown, // Wait for cool down
        failureCoolDown, // Wait for cool down
        abortCoolDown, // Wait for cool down
        finished, // Burn process & cool down finished
        aborted, // Burn process aborted, cool down finished
        failed, // Burn process failed, cool down finished
    };

    Incinerator();

    mode getMode() const;
    std::string getModeStr() const;
    bool isCoolingDown() const;
    bool isFinished() const;
    void init();
    void start();
    void abort();
    void reset();
    void task();

    BurnChamber burnerMain;
    BurnChamber burnerAft;
    AirPump airPump;

private:
    void fsm();
    void doReset();
    void doFail();
    void doAbort();
    std::string fmt_minutes(uint32_t seconds) const;

    bool _startFlag = false;
    bool _abortFlag = false;
    mode _mode = mode::idle;
    Timeout _timeout;
    uint32_t _burn_seconds, _burn_seconds_elapsed;
    float _tempMax;
};

extern Incinerator incinerator;
