#include "sys_config.h"

#include <vector>

ConfigItem::ConfigItem(std::string name, std::string desc, int32_t default_val)
:_name(name)
,_desc(desc)
,_val(default_val)
{
}

SysConfig::SysConfig()
{
    const std::vector<ConfigItem> initial = {
        { "foo", "Foo value", 42 },
        { "bar", "Bar stuff", 10 },
    };

    for (const auto& itm : initial) {
        _items.insert({ itm._name, itm });
    }
}

int32_t SysConfig::get(std::string name)
{
    auto it = _items.find(name);
    if (it == _items.end()) {
        return 0;
    }
    return it->second._val;
}

void SysConfig::set(std::string name, int32_t value)
{
    auto it = _items.find(name);
    if (it == _items.end()) {
        return;
    }
    it->second._val = value;
}

SysConfig sysconfig;
