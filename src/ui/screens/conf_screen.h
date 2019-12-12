#pragma once

#include <string>
#include <vector>
#include "screen.h"

class ConfItem
{
public:
    ConfItem(std::string helpText,
             std::string name,
             std::string desc,
             std::string unit,
             int step,
             int lowerBound,
             int upperBound);

    void draw(Adafruit_GFX& d, int16_t x, int16_t y, bool highlightName, bool highlightValue);
    void update(int digits);
    void set();

    std::string _helpText;
    std::string _name;
    std::string _desc;
    std::string _unit;
    int _val;
    int _step;
    int _lowerBound;
    int _upperBound;
};

class ConfScreen : public Screen
{
public:
    void reset() override;
    void draw() override;
    bool handleEncoderRotation(int delta) override;
    bool handleEncoderSwitch() override;

private:
    void drawItem(size_t idx, uint16_t x, uint16_t y);
    std::vector<ConfItem> _items;
    ssize_t _currItem;
    bool _enterValueMode;
};
