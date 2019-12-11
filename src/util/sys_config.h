#pragma once

#include <string>
#include <stdint.h>
#include <unordered_map>

#include <Preferences.h>

class SysConfig
{
public:
    SysConfig();

    void init();
    void reset();

    int32_t get(std::string name);
    void set(std::string name, int32_t value);

private:
    std::unordered_map<std::string, int32_t> _items;
    std::unordered_map<std::string, int32_t> _default;

    Preferences prefs;
};

extern SysConfig sysconfig;
