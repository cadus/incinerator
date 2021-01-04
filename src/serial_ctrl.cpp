#include "serial_ctrl.h"

#include <string>
#include <map>
#include <Arduino.h>

#include "base64.h"

#include "ui/screens/screen.h"
#include "util/syslog.h"
#include "incinerator/incinerator.h"

static void do_screenshot(std::string s)
{
    auto scr = Screen::screenshot();

    // Iterate through blocks of 768 bytes (will yield 1024 bytes encoded)
    while (scr.size()) {
        size_t len = scr.size();
        len = (len > 768) ? 768 : len;
        auto enc = base64::encode(&scr[0], len);
        printf("%s\r\n", enc.c_str());
        scr.erase(scr.begin(), scr.begin() + len);
    }
    printf("\r\n");
}

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
    { "SS", do_screenshot },
};

static void diag_handler(std::string cmd)
{
    cmd.erase(cmd.find_last_not_of(" \n\r\t")+1);
    printf("%s\r\n", cmd.c_str());
    size_t n = cmd.find(' ');
    const auto token1 = (n != cmd.npos) ? cmd.substr(0, n) : cmd;
    auto token2 = (n != cmd.npos) ? cmd.substr(n+1) : "";
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