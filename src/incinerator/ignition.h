#pragma once

#include <stdint.h>
#include <string>
#include "thermocouple.h"
#include "util/timeout.h"

#include "macros.h"

class Ignition
{
    NOT_COPYABLE(Ignition);
    NOT_MOVEABLE(Ignition);

    enum mode {
        idle,        // Idle / off
        start_set,   // Set ignition pulse
        start_wait,  // Wait until ignition period is done
        start_reset, // Reset ignition pulse
        check_wait,  // Wait for temperature rise
        check,       // Check temperature rise
        burning,     // Temperature check OK, Burning
        failure,     // Temperature check failed repeatedly
        stop,        // Stop ignition & reset state
    };

public:
    Ignition(std::string name, uint8_t pin, Thermocouple& thermocouple);
    mode getMode();
    void init();
    void start();
    void reset();
    bool ready();
    bool failed();
    void task();

private:
    const std::string _name;
    const uint8_t _pin;
    mode _mode;
    bool _startFlag;
    bool _resetFlag;
    Timeout _timeout;

    static constexpr uint32_t _pulse_ms = 100;
    static constexpr uint8_t _num_retries = 5;

    float _temp_begin;
    uint8_t _retries;
    static constexpr uint32_t _check_delay_ms = 5000;
    static constexpr float _check_delta_T = 10.f;

    Thermocouple& _thermocouple;
};
