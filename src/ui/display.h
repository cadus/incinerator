#pragma once

#include <GxEPD2_BW.h>

class Display
{
public:
    void init();
    void update();

private:
    static GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> _d;
};

extern Display display;
