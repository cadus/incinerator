#pragma once

#include <stdint.h>
#include <vector>
#include "Adafruit_GFX.h"
#include "GxEPD2.h"

class Icon {
public:
    Icon(uint16_t w, uint16_t h, const std::vector<uint8_t> d);

    // Draw icon to x, y
    void draw(Adafruit_GFX& gfx, int16_t x, int16_t y) const;
    // Draw icon centered to area
    void draw(Adafruit_GFX& gfx, int16_t x, int16_t y, int16_t w, int16_t h) const;

private:
    uint16_t width;
    uint16_t height;
    const std::vector<uint8_t> data;
};
