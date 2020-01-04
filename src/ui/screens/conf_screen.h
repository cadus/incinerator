#pragma once

#include "interactive_screen.h"
#include <string>
#include <vector>

class ConfItem : public ValueEntry
{
public:
    ConfItem(InteractiveScreen& parent,
             std::string helpText,
             std::string id,
             std::string desc,
             std::string unit,
             int step,
             int lowerBound,
             int upperBound,
             uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    virtual void update() override;

    std::string _helpText;
    std::string _id;
};

class ConfScreen : public InteractiveScreen
{
public:
    ConfScreen();

    void reset() override;
    void draw() override;

private:
    static constexpr uint16_t _dx = 160;
    static constexpr uint16_t _xs = 400 - _dx * 2;
    static constexpr uint16_t _ys = _content_y;
    static constexpr uint16_t _dy = 24;
};

extern ConfScreen confScreen;
