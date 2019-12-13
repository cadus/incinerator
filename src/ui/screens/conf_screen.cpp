#include "conf_screen.h"

#include <string>

#include "util/sys_config.h"

constexpr int16_t confRowHeight = 24;
constexpr int16_t confNameWidth = 120;
constexpr int16_t confValWidth = 80;

ConfItem::ConfItem(std::string helpText,
                   std::string name,
                   std::string desc,
                   std::string unit,
                   int step,
                   int lowerBound,
                   int upperBound)
:_helpText(helpText)
,_name(name)
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
    Screen::PrintFlags flags;
    flags.set(Screen::PrintFlag::justifyRight);
    if (highlightName) {
        flags.set(Screen::PrintFlag::invert);
    }
    Screen::print(_desc, x + 20, y, confNameWidth - 20, confRowHeight, flags);

    x += confNameWidth;
    x += 8;

    flags = 0;
    flags.set(Screen::PrintFlag::justifyLeft);
    if (highlightValue) {
        flags.set(Screen::PrintFlag::invert);
    }
    char tmp[80];
    snprintf(tmp, sizeof(tmp), "%d%s", _val, _unit.c_str());
    Screen::print(tmp, x, y, confValWidth - 20, confRowHeight, flags);
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
        { "Low temp.threshold to open valve", "main_T_low", "temp.low", "C", 10, 300, 700 },
        { "High temp.threshold to close valve", "main_T_high", "temp.high", "C", 10, 300, 700 },
        { "Ignition control timeout", "main_ign_t", "ign.time", "s", 1, 1, 15 },
        { "Ignition control temp.delta", "main_ign_delta", "ign.delta", "C", 1, 1, 100 },
        { "Low temp.threshold to open valve",  "aft_T_low", "temp.low", "C", 10, 1000, 1500 },
        { "High temp.threshold to close valve", "aft_T_high", "temp.high", "C", 10, 1000, 1500 },
        { "Ignition control timeout", "aft_ign_t", "ign.time", "s", 1, 1, 15 },
        { "Ignition control temp.delta", "aft_ign_delta", "ign.delta", "C", 1, 1, 100 },
        { "Air pump speed", "airpmp_speed", "pumpspd", "%", 1, 0, 100 },
        { "Buzzer volume", "buzzer_vol", "buzz.vol", "", 1, 0, 7 }
    };
    _currItem = 0;
    _enterValueMode = false;
}

void ConfScreen::drawItem(size_t idx, uint16_t x, uint16_t y)
{
    _items[idx].draw(_d, x, y,
                     !_enterValueMode && _currItem == idx,
                     _enterValueMode && _currItem == idx);
}

void ConfScreen::draw()
{
    uint16_t y = _content_y + 2;

    print("Main burner settings:", 0, y, _d.width(), confRowHeight);
    y += confRowHeight;
    drawItem(0, 0, y);
    drawItem(1, 0, y + confRowHeight);
    drawItem(2, 200, y);
    drawItem(3, 200, y + confRowHeight);
    y += confRowHeight * 2;

    y += 1;
    _d.drawFastHLine(10, y, _d.width()-20, GxEPD_BLACK);
    y += 1;

    print("Afterburner settings:", 0, y, _d.width(), confRowHeight);
    y += confRowHeight;
    drawItem(4, 0, y);
    drawItem(5, 0, y + confRowHeight);
    drawItem(6, 200, y);
    drawItem(7, 200, y + confRowHeight);
    y += confRowHeight * 2;

    y += 1;
    _d.drawFastHLine(10, y, _d.width()-20, GxEPD_BLACK);
    y += 1;

    print("Misc.settings:", 0, y, _d.width(), confRowHeight);
    y += confRowHeight;
    drawItem(8, 0, y);
    drawItem(9, 200, y);
    y += confRowHeight;

    y += 1;
    _d.drawFastHLine(10, y, _d.width()-20, GxEPD_BLACK);
    y += 3;

    PrintFlags flags;
    if (_currItem == _items.size()) {
        flags.set(PrintFlag::invert);
    }
    print("Apply", 125, y, _d.width() - 250, confRowHeight, flags);

    if (_currItem < _items.size()) {
        setStatus(_items[_currItem]._helpText);
    } else {
        setStatus("Close settings screen");
    }
}

bool ConfScreen::handleEncoderRotation(int delta)
{
    const ssize_t range = _items.size() + 1;
    if (!_enterValueMode) {
        _currItem += delta;
        while (_currItem < 0) {
            _currItem += range;
        }
        while (_currItem >= range) {
            _currItem -= range;
        }
    } else if (_currItem >= 0 && _currItem < _items.size()) {
        _items[_currItem].update(delta);
    }
    return true;
}

bool ConfScreen::handleEncoderSwitch()
{
    if (_currItem == _items.size()) {
        // TODO: close screen
        return true;
    }

    if (_enterValueMode) {
        if (_currItem >= 0 && _currItem < _items.size()) {
            _items[_currItem].set();
        }
    }
    _enterValueMode = !_enterValueMode;
    return true;
}