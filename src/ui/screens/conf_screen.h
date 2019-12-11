#pragma once

#include "screen.h"

class ConfScreen : public Screen
{
public:
    void draw() override;
    bool handleEncoderRotation(int delta) override;
    bool handleEncoderSwitch() override;

private:
    int _test;
};
