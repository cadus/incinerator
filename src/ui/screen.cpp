#include "screen.h"

#include <Fonts/FreeMonoBold9pt7b.h>

#include "ui/icons.h"
#include "debounced_encoder.h"
#include "incinerator/incinerator.h"

#include "hw_config.h"

extern void GxEPD2_busyWaitCallback();

GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> Screen::_d(GxEPD2_420(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

Screen::Screen()
:_progressPercent(0.f)
,_statusStr("")
{
}

void Screen::init()
{
    // init display
    _d.init();
    _d.setRotation(0);
    _d.setFullWindow();
    _d.fillScreen(GxEPD_WHITE);
    _d.display(false); // full update
}

void Screen::setProgress(float percent)
{
    _progressPercent = percent;
}

void Screen::update()
{
    constexpr uint32_t top_bar_height = 40;
    constexpr uint32_t bottom_bar_height = 36;
    constexpr uint32_t line_margin = 3;

    constexpr uint32_t icon_box_width = 38;
    constexpr uint32_t temp_box_width = 64;

    _d.setRotation(0);
    _d.setFont(&FreeMonoBold9pt7b);
    _d.setTextColor(GxEPD_BLACK);
    _d.setPartialWindow(0, 0, _d.width(), _d.height());
    unsigned long start = micros();

    _d.fillScreen(GxEPD_WHITE);

    uint16_t x = 0;
    _d.drawBitmap(x + (icon_box_width - ICON_THERMOMETER_WIDTH) / 2,
                  (top_bar_height - ICON_THERMOMETER_HEIGHT) / 2,
                  ICON_THERMOMETER_DATA,
                  ICON_THERMOMETER_WIDTH,
                  ICON_THERMOMETER_HEIGHT,
                  GxEPD_BLACK);

    x += icon_box_width;

    for (int i = 0; i < 2; i++) {
        const BurnChamber& bch = (i == 0) ? burner_main : burner_aft;
        char tmp[16] = { 0 };
        snprintf(tmp, sizeof(tmp), "%d", int((bch.getTemp().internal + 100.0) * (i ? 10.0 : 1.0)));
        int16_t x1, y1;
        uint16_t w, h;
        _d.getTextBounds(tmp, 0, 0, &x1, &y1, &w, &h);        
        _d.setCursor(x + (temp_box_width - w) / 2 - x1,
                     (top_bar_height - y1) / 2);
        _d.print(tmp);

        x += temp_box_width;
        _d.drawFastVLine(x, line_margin, top_bar_height - (line_margin * 2), GxEPD_BLACK);
    }

    x += line_margin;
    _d.drawFastVLine(x, line_margin, top_bar_height - (line_margin * 2), GxEPD_BLACK);

    _d.drawBitmap(x + (icon_box_width - ICON_CLOCK_WIDTH) / 2,
                  (top_bar_height - ICON_CLOCK_HEIGHT) / 2,
                  ICON_CLOCK_DATA,
                  ICON_CLOCK_WIDTH,
                  ICON_CLOCK_HEIGHT,
                  GxEPD_BLACK);
    
    x += icon_box_width;
    
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

    _d.drawBitmap(x + (icon_box_width - ICON_FLAME_WIDTH) / 2,
                  (top_bar_height - ICON_FLAME_HEIGHT) / 2,
                  ICON_FLAME_DATA,
                  ICON_FLAME_WIDTH,
                  ICON_FLAME_HEIGHT,
                  GxEPD_BLACK);
    
    _d.drawFastHLine(line_margin, top_bar_height, _d.width() - (line_margin * 2), GxEPD_BLACK);

    _d.drawFastHLine(line_margin, _d.height() - bottom_bar_height, _d.width() - (line_margin * 2), GxEPD_BLACK);

    int16_t x1, y1;
    uint16_t w, h;
    _d.getTextBounds(_statusStr.c_str(), 0, 0, &x1, &y1, &w, &h);        
    _d.setCursor((_d.width() - w) / 2 - x1,
                 _d.height() - bottom_bar_height + (bottom_bar_height - y1) / 2);
    _d.print(_statusStr.c_str());

    _d.display(true); // partial update

    unsigned long elapsed = micros() - start;
    Serial.print("time spent - w/o refresh: ");
    Serial.print(elapsed - 560000);
    Serial.println(" us");
    Serial.print("time spent - total: ");
    Serial.print(elapsed);
    Serial.println(" us");
}

Screen screen;
