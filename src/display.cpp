#include "display.h"

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "hw_config.h"
#include "icons.h"
#include "thermocouple.h"

#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
GxEPD2_BW<GxEPD2_420, MAX_HEIGHT(GxEPD2_420)> display(GxEPD2_420(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

void display_init()
{
    display.init(115200);
    display.setRotation(0);
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());
}

void display_redraw()
{
    static int cnt = 0;
    cnt++;

    display.setRotation(0);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);

    thermocouple_meas_t T = thermocouple_get();
    String str = "T_int: "
                 + String(T.temp_internal, 2)
                 + ", T_ext:"
                 + String(T.temp_external, 2)
                 + ", cnt: "
                 + String(cnt);

    display.setPartialWindow(0, 0, display.width(), display.height());
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(10, 15);
        display.print(str);

        display.drawBitmap(100, 100, ICON_test_DATA, ICON_test_WIDTH, ICON_test_HEIGHT, GxEPD_BLACK);
    } while (display.nextPage());
}
