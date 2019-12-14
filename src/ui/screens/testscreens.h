#pragma once

#include "interactive_screen.h"

class FooScreen : public InteractiveScreen
{
public:
    void reset() override;
private:
    bool action1();
    bool action2();
};
