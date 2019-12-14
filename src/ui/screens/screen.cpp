#include "screen.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "ui/icons.h"
#include "incinerator/incinerator.h"

#include "hw_config.h"

extern void GxEPD2_busyWaitCallback();

GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> Screen::_d(GxEPD2_420(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
float Screen::_progressPercent = 0.f;
std::string Screen::_statusStr = "";

Screen::Screen()
:_nextScreen(nullptr)
{
}

void Screen::init()
{
    // init display
    _d.init();
}

void Screen::setProgress(float percent)
{
    _progressPercent = percent;
}

void Screen::setStatus(const std::string s)
{
    _statusStr = s;
}

void Screen::setNextScreen(Screen *nextScreen)
{
    _nextScreen = nextScreen;
}

Screen *Screen::getNextScreen()
{
    return _nextScreen;
}

void Screen::print(const std::string s, uint16_t x, uint16_t y, uint16_t w, uint16_t h, PrintFlags flags)
{
    constexpr uint16_t margin = 4;
    if (flags[PrintFlag::drawRect]) {
        _d.drawRect(x - margin, y, w + margin*2, h, GxEPD_BLACK);
    }
    if (flags[PrintFlag::invert]) {
        _d.fillRect(x - margin, y, w + margin*2, h, GxEPD_BLACK);
        _d.setTextColor(GxEPD_WHITE);
        _d.setFont(&FreeSansBold9pt7b);
    } else {
        _d.setTextColor(GxEPD_BLACK);
        _d.setFont(flags[PrintFlag::bold] ? &FreeSansBold9pt7b : &FreeSans9pt7b);
    }
    int16_t x1, y1;
    uint16_t wT, hT;
    _d.getTextBounds(s.c_str(), 0, 0, &x1, &y1, &wT, &hT);
    if (flags[PrintFlag::justifyLeft]) {
        _d.setCursor(x - x1,
                    y + 2 * h / 3 + 1);
    } else if (flags[PrintFlag::justifyRight]) {
        _d.setCursor(x + (w - wT) - x1,
                    y + 2 * h / 3 + 1);
    } else {
        _d.setCursor(x + (w - wT) / 2 - x1,
                    y + 2 * h / 3 + 1);
    }
    _d.print(s.c_str());
}

void Screen::update(bool fullRefresh)
{
    unsigned long start = millis();

    constexpr uint32_t top_bar_height = 40;
    constexpr uint32_t bottom_bar_height = 32;
    constexpr uint32_t line_margin = 3;

    constexpr uint32_t icon_box_width = 38;
    constexpr uint32_t temp_box_width = 64;

    // Init screen drawing
    _d.setRotation(0);
    _d.setPartialWindow(0, 0, _d.width(), _d.height());
    _d.fillScreen(GxEPD_WHITE);

    // Draw top bar. Start at the left side
    uint16_t x = 0;
    // Draw thermometer icon
    _d.drawBitmap(x + (icon_box_width - ICON_THERMOMETER_WIDTH) / 2,
                  (top_bar_height - ICON_THERMOMETER_HEIGHT) / 2,
                  ICON_THERMOMETER_DATA,
                  ICON_THERMOMETER_WIDTH,
                  ICON_THERMOMETER_HEIGHT,
                  GxEPD_BLACK);
    x += icon_box_width;

    // Print burn chamber temperatures
    for (int i = 0; i < 2; i++) {
        PrintFlags flags;
        flags.set(PrintFlag::bold);
        const BurnChamber& bch = (i == 0) ? burner_main : burner_aft;
        char tmp[16] = { 0 };
        snprintf(tmp, sizeof(tmp), "%d", int(bch.getTemp().external));
        print(tmp, x, 0, temp_box_width, top_bar_height, flags);

        x += temp_box_width;
        _d.drawFastVLine(x, line_margin, top_bar_height - (line_margin * 2), GxEPD_BLACK);
    }

    // Draw 2nd line after temperature readouts
    x += line_margin;
    _d.drawFastVLine(x, line_margin, top_bar_height - (line_margin * 2), GxEPD_BLACK);

    // Draw stopwatch icon
    _d.drawBitmap(x + (icon_box_width - ICON_CLOCK_WIDTH) / 2,
                  (top_bar_height - ICON_CLOCK_HEIGHT) / 2,
                  ICON_CLOCK_DATA,
                  ICON_CLOCK_WIDTH,
                  ICON_CLOCK_HEIGHT,
                  GxEPD_BLACK);
    x += icon_box_width;
    
    // Draw progress bar
    for (int i = 0; i < 10; i++) {
        bool filled = _progressPercent >= (100.f / 10.f) * (i+1);
        _d.drawBitmap(x,
                    (top_bar_height - ICON_BOX_CLEAR_HEIGHT) / 2,
                    filled ? ICON_BOX_FILLED_DATA : ICON_BOX_CLEAR_DATA,
                    ICON_BOX_CLEAR_WIDTH,
                    ICON_BOX_CLEAR_HEIGHT,
                    GxEPD_BLACK);
        x += ICON_BOX_CLEAR_WIDTH;
    }

    // Draw flame to the right
    _d.drawBitmap(x + (icon_box_width - ICON_FLAME_WIDTH) / 2,
                  (top_bar_height - ICON_FLAME_HEIGHT) / 2,
                  ICON_FLAME_DATA,
                  ICON_FLAME_WIDTH,
                  ICON_FLAME_HEIGHT,
                  GxEPD_BLACK);

    // Draw line below top bar
    _d.drawFastHLine(line_margin, top_bar_height, _d.width() - (line_margin * 2), GxEPD_BLACK);

    // Draw line on top of bottom bar
    _d.drawFastHLine(line_margin, _d.height() - bottom_bar_height, _d.width() - (line_margin * 2), GxEPD_BLACK);

    // Let the subclass screen do its thing
    _d.setFont(&FreeSans9pt7b);
    draw();

    // Print status string on bottom bar
    print(_statusStr, 0, _d.height() - bottom_bar_height, _d.width(), bottom_bar_height);

    // partial update or full update if necessary
    _d.display(!fullRefresh);

    unsigned long elapsed = millis() - start;
    Serial.printf("time spent: %lu ms\r\n", elapsed);
}
