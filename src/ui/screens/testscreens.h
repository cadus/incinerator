#pragma once

#include "interactive_screen.h"

class FooScreen : public InteractiveScreen
{
public:
    void reset() override;
};

class BarScreen : public InteractiveScreen
{
public:
    void reset() override;
};

class TestScreen : public InteractiveScreen
{
public:
    void reset() override;
};

extern FooScreen fooScreen;
extern BarScreen barScreen;
extern TestScreen testScreen;
