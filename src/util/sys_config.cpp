#include "sys_config.h"

#include <vector>

SysConfig::SysConfig()
:_default({
    { "main_T_low", 500 },
    { "main_T_high", 550 },
    { "aft_T_low", 1200 },
    { "aft_T_high", 1250 },
    { "main_ign_t", 5 },
    { "main_ign_delta", 5 },
    { "aft_ign_t", 5 },
    { "aft_ign_delta", 5 },
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
