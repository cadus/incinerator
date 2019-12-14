#include "conf_screen.h"

#include <string>

#include "util/sys_config.h"
#include "testscreens.h"

constexpr int16_t confRowHeight = 24;
constexpr int16_t confNameWidth = 120;
constexpr int16_t confValWidth = 80;

ConfItem::ConfItem(InteractiveScreen& parent,
                   std::string helpText,
                   std::string id,
                   std::string desc,
                   std::string unit,
                   int step,
                   int lowerBound,
                   int upperBound,
                   uint16_t x, uint16_t y, uint16_t w, uint16_t h)
:ValueEntry(parent, desc, unit,
            sysconfig.get(id), step, lowerBound, upperBound,
            x, y, w, h)
,_helpText(helpText)
,_id(id)
{
}

void ConfItem::draw(bool selected)
{
    if (selected) {
        _parent.setStatus(_helpText);
    }
    ValueEntry::draw(selected);
}

void ConfItem::update()
{
    sysconfig.set(_id, _val);
}

void ConfScreen::reset()
{
    constexpr uint16_t x = 0;
    constexpr uint16_t dx = 200;
    constexpr uint16_t y = _content_y;
    constexpr uint16_t dy = confRowHeight;

    static std::vector<ConfItem> confs = {
        {
            *this, "Low temp.threshold to open valve",
            "main_T_low", "temp.low", "C", 10, 300, 700,
            x + dx * 0, y + dy * 0, dx, dy
        },
        {
            *this, "High temp.threshold to close valve",
            "main_T_high", "temp.high", "C", 10, 300, 700,
            x + dx * 1, y + dy * 0, dx, dy
        },
        {
            *this, "Ignition control timeout",
            "main_ign_t", "ign.time", "s", 1, 1, 15,
            x + dx * 0, y + dy * 1, dx, dy
        },
        {
            *this, "Ignition control temp.delta",
            "main_ign_delta", "ign.delta", "C", 1, 1, 100,
            x + dx * 1, y + dy * 1, dx, dy
        },
        {
            *this, "Low temp.threshold to open valve",
            "aft_T_low", "temp.low", "C", 10, 1000, 1500,
            x + dx * 0, y + dy * 2, dx, dy
        },
        {
            *this, "High temp.threshold to close valve",
            "aft_T_high", "temp.high", "C", 10, 1000, 1500,
            x + dx * 1, y + dy * 2, dx, dy
        },
        {
            *this, "Ignition control timeout",
            "aft_ign_t", "ign.time", "s", 1, 1, 15,
            x + dx * 0, y + dy * 3, dx, dy
        },
        {
            *this, "Ignition control temp.delta",
            "aft_ign_delta", "ign.delta", "C", 1, 1, 100,
            x + dx * 1, y + dy * 3, dx, dy
        },
        {
            *this, "Air pump speed",
            "airpmp_speed", "pumpspd", "%", 1, 0, 100,
            x + dx * 0, y + dy * 4, dx, dy
        },
        {
            *this, "Buzzer volume",
            "buzzer_vol", "buzz.vol", "", 1, 0, 7,
            x + dx * 1, y + dy * 4, dx, dy
        },
    };

    static ScreenChangeButton ok(*this, "OK", &barScreen, 125, y+dy*5, 150, dy);

    _items.clear();
    for (auto& c : confs) {
        _items.push_back(&c);
    }
    _items.push_back(&ok);

    InteractiveScreen::reset();
}

void ConfScreen::draw()
{
    InteractiveScreen::draw();
}

ConfScreen confScreen;
