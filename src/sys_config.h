#pragma once

#include <string>
#include <stdint.h>
#include <unordered_map>

#include "macros.h"

class ConfigItem
{
public:
    ConfigItem(std::string name, std::string desc, int32_t default_val);

    std::string _name;
    std::string _desc;
    int32_t _val;
};

class SysConfig
{
public:
    SysConfig();

    int32_t get(std::string name);
    void set(std::string name, int32_t value);

private:
    std::unordered_map<std::string, ConfigItem> _items;
};

extern SysConfig sysconfig;
