#pragma once

#include <string>
#include <GxEPD2_BW.h>

#include "macros.h"

class Screen
{
    NOT_COPYABLE(Screen);
    NOT_MOVEABLE(Screen);

public:
    Screen();

    void init();
    void update();

    void setProgress(float percent);

private:
    static GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> _d;
    float _progressPercent;
    std::string _statusStr;
};

extern Screen screen;
