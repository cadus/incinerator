#include "syslog.h"

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#include "ui/screens/screen.h"

static constexpr log_level_t lvl_console = LOG_DEBUG;
static constexpr log_level_t lvl_display = LOG_INFO;

void syslog(log_level_t lvl, const char *fmt, ...)
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
