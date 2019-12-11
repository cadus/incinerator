#pragma once

#include <string>
#include <GxEPD2_BW.h>

#include "macros.h"

class Screen
{
public:
    Screen();

    void init();
    void update();

    void setProgress(float percent);

    virtual void draw() {};

protected:
    static GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> _d;

    // space reserved for content screen (subclass)
    static constexpr uint16_t _content_y = 40;
    static constexpr uint16_t _content_height = 300 - 76;

private:
    float _progressPercent;
    std::string _statusStr;
};
