#pragma once

#include <string>
#include <bitset>
#include <GxEPD2_BW.h>

#include "macros.h"

class Screen
{
public:
    Screen();

    static void init();
    void update();

    void setProgress(float percent);
    void setStatus(const std::string s);
    Screen *nextScreen();

    virtual void reset() = 0;
    virtual void draw() = 0;
    virtual bool handleEncoderRotation(int delta) = 0;
    virtual bool handleEncoderSwitch() = 0;

    enum PrintFlag {
        justifyLeft,
        justifyRight,
        drawRect,
        invert,
        numFlags
    };
    typedef std::bitset<PrintFlag::numFlags> PrintFlags;

    static void print(const std::string s, uint16_t x, uint16_t y, uint16_t w, uint16_t h, PrintFlags flags = 0);

protected:
    static GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> _d;

    // space reserved for content screen (subclass)
    static constexpr uint16_t _content_y = 40;
    static constexpr uint16_t _content_height = 300 - 76;

    Screen *_nextScreen;

private:
    static float _progressPercent;
    static std::string _statusStr;
};
