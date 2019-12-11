#include "conf_screen.h"

#include <string>

#include "util/sys_config.h"

constexpr int16_t confRowHeight = 30;
constexpr int16_t confNameWidth = 100;
constexpr int16_t confValWidth = 70;
constexpr int16_t confUnitWidth = 40;

ConfItem::ConfItem(std::string name,
                   std::string desc,
                   std::string unit,
                   int step,
                   int lowerBound,
                   int upperBound)
:_name(name)
,_desc(desc)
,_unit(unit)
,_val(sysconfig.get(name))
,_step(step)
,_lowerBound(lowerBound)
,_upperBound(upperBound)
{
}

void ConfItem::draw(Adafruit_GFX& d, int16_t x, int16_t y, bool highlightName, bool highlightValue)
{
    int16_t x1, y1;
    uint16_t w, h;

    Serial.printf("drawing %s\r\n", _name.c_str());

    d.getTextBounds(_desc.c_str(), 0, 0, &x1, &y1, &w, &h);        
    d.setCursor(x + (confNameWidth - w) / 2 - x1,
                (confRowHeight - y1) / 2);
    d.print(_desc.c_str());
    if (highlightName) {
        d.drawRect(x, y, x + confNameWidth - 1, y + confRowHeight - 1, GxEPD_BLACK);
    }
    x += confNameWidth;

    String val(_val);
    d.getTextBounds(val, 0, 0, &x1, &y1, &w, &h);        
    d.setCursor(x + (confValWidth - w) / 2 - x1,
                (confRowHeight - y1) / 2);
    d.print(_val);
    if (highlightValue) {
        d.drawRect(x, y, x + confValWidth - 1, y + confRowHeight - 1, GxEPD_BLACK);
    }
    x += confValWidth;

    d.getTextBounds(_unit.c_str(), 0, 0, &x1, &y1, &w, &h);        
    d.setCursor(x + (confNameWidth - w) / 2 - x1,
                (confRowHeight - y1) / 2);
    d.print(_unit.c_str());
}

void ConfItem::update(int digits)
{
    _val += digits * _step;
}

void ConfItem::set()
{
    sysconfig.set(_name, _val);
}

void ConfScreen::reset()
{
    _items = {
        { "name1", "desc1", "unit1", 1, 0, 100},
        { "name2", "desc2", "unit2", 1, 0, 100},
        { "name3", "desc3", "unit3", 1, 0, 100},
    };
    _currItem = 0;
    _enterValueMode = false;
}

void ConfScreen::draw()
{
    uint16_t y = _content_y;
    size_t k = 0;
    for (auto& i : _items) {
        i.draw(_d, 0, y,
               !_enterValueMode && _currItem == k,
               _enterValueMode && _currItem == k);
        y += confRowHeight;
        k++;
    }
}

bool ConfScreen::handleEncoderRotation(int delta)
{
    if (!_enterValueMode) {
        _currItem += delta;
        _currItem = max((size_t)0, _currItem);
        _currItem = min(_items.size() - 1, _currItem);
    } else {
        _items[_currItem].update(delta);
    }
    return true;
}

bool ConfScreen::handleEncoderSwitch()
{
    _enterValueMode = !_enterValueMode;
    return true;
}