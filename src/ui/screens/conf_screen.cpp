#include "conf_screen.h"

#include <string>

#include "util/sys_config.h"

constexpr int16_t confRowHeight = 24;
constexpr int16_t confNameWidth = 120;
constexpr int16_t confValWidth = 72;

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
    d.setCursor(x + (confNameWidth - w) - x1,
                y + (confRowHeight - y1) / 2);
    d.print(_desc.c_str());
    if (highlightName) {
        d.drawRect(x + 2, y + 2, confNameWidth + 2, confRowHeight - 2, GxEPD_BLACK);
    }
    x += confNameWidth;
    x += 8;

    char tmp[80];
    snprintf(tmp, sizeof(tmp), "%d %s", _val, _unit.c_str());
    d.getTextBounds(tmp, 0, 0, &x1, &y1, &w, &h);        
    d.setCursor(x,
                y + (confRowHeight - y1) / 2);
    d.print(tmp);
    if (highlightValue) {
        d.drawRect(x - 4, y + 2, confValWidth - 2, confRowHeight - 2, GxEPD_BLACK);
    }
}

void ConfItem::update(int digits)
{
    _val += digits * _step;
    _val = min(_upperBound, _val);
    _val = max(_lowerBound, _val);
}

void ConfItem::set()
{
    sysconfig.set(_name, _val);
}

void ConfScreen::reset()
{
    _items = {
        { "main_T_low", "temp.low", "C", 10, 300, 700 },
        { "main_T_high", "temp.high", "C", 10, 300, 700 },
        { "main_ign_t", "ign.time", "s", 1, 1, 15 },
        { "main_ign_delta", "ign.delta", "C", 1, 1, 100 },
        { "aft_T_low", "temp.low", "C", 1, 1000, 1500 },
        { "aft_T_high", "temp.high", "C", 1, 1000, 1500 },
        { "aft_ign_t", "ign.time", "s", 1, 1, 15 },
        { "aft_ign_delta", "ign.delta", "C", 1, 1, 100 },
//        { "airpmp_speed", "pumpspd", "%", 1, 0, 100 },
//        { "buzzer_vol", "buzz.vol", "", 1, 0, 7 }
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
        while (_currItem < 0) {
            _currItem += _items.size();
        }
        while (_currItem >= _items.size()) {
            _currItem -= _items.size();
        }
    } else {
        _items[_currItem].update(delta);
    }
    return true;
}

bool ConfScreen::handleEncoderSwitch()
{
    if (_enterValueMode) {
        if (_currItem >= 0 && _currItem < _items.size()) {
            _items[_currItem].set();
        }
    }
    _enterValueMode = !_enterValueMode;
    return true;
}