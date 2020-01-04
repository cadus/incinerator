#pragma once

#include "interactive_screen.h"

class TestButton : public PushButton
{
public:
    TestButton(InteractiveScreen& parent,
               std::string text,
               uint16_t x, uint16_t y, uint16_t w, uint16_t h,
               std::string helpText = "");

    virtual void draw(bool selected) override;

private:
    bool handler();
    virtual void toggle() = 0;
    virtual std::string getState() = 0;
};

class AirPumpTest : public TestButton
{
public:
    AirPumpTest(InteractiveScreen& parent,
                uint16_t x, uint16_t y, uint16_t w, uint16_t h);

private:
    virtual void toggle() override;
    virtual std::string getState() override;
};

class TestScreen : public InteractiveScreen
{
public:
    TestScreen();

    void reset() override;

private:
    static constexpr uint16_t _dx = 160;
    static constexpr uint16_t _xs = 400 - _dx * 2;
    static constexpr uint16_t _ys = _content_y;
    static constexpr uint16_t _dy = 24;
};

extern TestScreen testScreen;
