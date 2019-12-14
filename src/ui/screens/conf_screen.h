#pragma once

#include <string>
#include <vector>
#include "interactive_screen.h"

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

    virtual void draw(bool selected) override;
    virtual void update() override;

    std::string _helpText;
    std::string _id;
};

class ConfScreen : public InteractiveScreen
{
public:
    void reset() override;
    void draw() override;
};

extern ConfScreen confScreen;
