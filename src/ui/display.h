#pragma once

#include <GxEPD2_BW.h>

class Display
{
public:
    void init();
    void update();

    void setProgress(float percent);

private:
    static GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> _d;
    float _progress_percent;
};

extern Display display;
