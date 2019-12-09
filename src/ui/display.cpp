#include "display.h"

#include <Fonts/FreeMonoBold9pt7b.h>

#include "ui/icons.h"
#include "debounced_encoder.h"
#include "incinerator/incinerator.h"

#include "hw_config.h"

extern void GxEPD2_busyWaitCallback();

GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> Display::_d(GxEPD2_420(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

void Display::init()
{
    _d.init();
    _d.setRotation(0);
    _d.setFullWindow();
    _d.fillScreen(GxEPD_WHITE);
    _d.display(false); // full update
}

void Display::update()
{
    static int cnt = 0;
    cnt++;

    _d.setRotation(0);
    _d.setFont(&FreeMonoBold9pt7b);
    _d.setTextColor(GxEPD_BLACK);
    _d.setPartialWindow(0, 0, _d.width(), _d.height());
    unsigned long start = micros();

    _d.fillScreen(GxEPD_WHITE);
    _d.drawBitmap(0, 0, ICON_THERMOMETER_DATA, ICON_THERMOMETER_WIDTH, ICON_THERMOMETER_HEIGHT, GxEPD_BLACK);

    _d.display(true); // partial update

    unsigned long elapsed = micros() - start;
    Serial.print("time spent - w/o refresh: ");
    Serial.print(elapsed - 560000);
    Serial.println(" us");
    Serial.print("time spent - total: ");
    Serial.print(elapsed);
    Serial.println(" us");
}

Display display;
