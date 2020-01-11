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

#include <Arduino.h>

#include "incinerator/incinerator.h"

#include "ui/buzzer.h"
#include "ui/debounced_encoder.h"
#include "ui/ui.h"

#include "util/sys_config.h"
#include "util/timeout.h"

#include "hw_config.h"
#include "version.h"

static void IRAM_ATTR timer_isr(void);
static hw_timer_t* timer = NULL;

static Ui ui;

void setup()
{
    Serial.begin(115200);

    sysconfig.init();

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, timer_isr, true);
    timerAlarmWrite(timer, 250, true);

    delay(100);
    ui.init();

    incinerator.init();

    timerAlarmEnable(timer);

    Serial.printf("Incinerator version %s, build date %s\r\n", gitversion, builddate);
}

void background_task()
{
    incinerator.task();
    ui.backgroundTask();
}

void GxEPD2_busyWaitCallback()
{
    background_task();
}

void loop()
{
    ui.task();
    background_task();
}

static void IRAM_ATTR timer_isr(void)
{
    // every 250 us
    encoder_check_rotation();
    encoder_check_switch();

    static uint8_t count = 0;
    if (!(count++ & 3)) {
        // once per millisecond
        buzzer.task();
        Timeout::incrementMs();
    }
}
