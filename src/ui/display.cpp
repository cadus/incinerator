#include "display.h"

#include <Fonts/FreeMonoBold9pt7b.h>

#include "icons.h"
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

    thermocouple_meas_t T = burner_main.getTemp();
    String str = "I:"
                 + String(T.internal, 2)
                 + ", E:"
                 + String(T.external, 2)
                 + ", e: "
                 + String(encoder_position())
                 + ", c: "
                 + String(cnt % 100);

    const uint8_t ep = max(min(encoder_position(), 25), 0);

    _d.setPartialWindow(0, 0, _d.width(), _d.height());
    unsigned long start = micros();

    _d.fillScreen(GxEPD_WHITE);
    _d.setCursor(10, 15);
    _d.print(str);
    for (uint8_t i = 0; i < 25; i++) {
        const uint8_t *icon = (ep > i) ? ICON_BOX_FILLED_DATA : ICON_BOX_CLEAR_DATA;
        _d.drawBitmap(i * ICON_BOX_CLEAR_WIDTH, 20, icon, ICON_BOX_CLEAR_WIDTH, ICON_BOX_CLEAR_HEIGHT, GxEPD_BLACK);
    }
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
