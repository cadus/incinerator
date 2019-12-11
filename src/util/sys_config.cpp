#include "sys_config.h"

#include <vector>

SysConfig::SysConfig()
:_default({
    {"foo", 42},
    {"bar", 10},
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
    prefs.putInt(it->first.c_str(), it->second);
}

SysConfig sysconfig;
