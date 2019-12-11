#pragma once

#include <string>
#include <GxEPD2_BW.h>

#include "macros.h"

class Screen
{
public:
    static void init();
    void update();

    void setProgress(float percent);

    virtual void draw() = 0;
    virtual bool handleEncoderRotation(int delta) = 0;
    virtual bool handleEncoderSwitch() = 0;

protected:
    static GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> _d;

    // space reserved for content screen (subclass)
    static constexpr uint16_t _content_y = 40;
    static constexpr uint16_t _content_height = 300 - 76;

private:
    static float _progressPercent;
    static std::string _statusStr;
};
