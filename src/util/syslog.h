#pragma once

enum log_level_t {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};

void syslog(enum log_level_t lvl, const char* fmt, ...);
