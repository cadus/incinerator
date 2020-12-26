#include "serial_ctrl.h"

#include <string>
#include <map>
#include <Arduino.h>

#include "util/syslog.h"
#include "incinerator/incinerator.h"

#ifdef ENABLE_TEMP_SIMULATION
static void sim_temp(BurnChamber& b, std::string s)
{
    int t = atoi(s.c_str());
    if (!t) {
        return;
    }
    b.thermocouple.sim_temp = t;
}

static void sim_temp_main(std::string s)
{
    sim_temp(incinerator.burnerMain, s);
}

static void sim_temp_aft(std::string s)
{
    sim_temp(incinerator.burnerAft, s);
}
#endif

typedef void (*handler_t)(std::string);

static const std::map<std::string, handler_t> commands = {
#ifdef ENABLE_TEMP_SIMULATION
    { "TM", sim_temp_main },
    { "TA", sim_temp_aft },
#endif
};

static void diag_handler(std::string cmd)
{
    cmd.erase(cmd.find_last_not_of(" \n\r\t")+1);
    size_t n = cmd.find(' ');
    if (n == cmd.npos) {
        return;
    }
    const auto token1 = cmd.substr(0, n);
    auto token2 = cmd.substr(n+1);
    auto it = commands.find(token1);
    if (it == commands.end()) {
        return;
    }
    it->second(token2);
}

void serial_ctrl_task()
{
    static std::string rx;
    while (Serial.available()) {
        int c = Serial.read();
        if (c >= 0) {
            rx += (char)c;
            if (c == '\r' || c == '\n') {
                diag_handler(rx);
                rx.clear();
            }
        }
    }
}