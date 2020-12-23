#include "icon.h"

Icon::Icon(uint16_t w, uint16_t h, const std::vector<uint8_t> d)
: width(w)
, height(h)
, data(d)
{
}

void Icon::draw(Adafruit_GFX& gfx, int16_t x, int16_t y) const
{
    gfx.drawBitmap(x, y, &data[0], width, height, GxEPD_BLACK);
}

void Icon::draw(Adafruit_GFX& gfx, int16_t x, int16_t y, int16_t w, int16_t h) const
{
    draw(gfx, x + (w - width) / 2, y + (h - height) / 2);
}
