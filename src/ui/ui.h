#pragma once

#include "ui/screens/screen.h"
#include "ui/screens/conf_screen.h"

class Ui
{
public:
    void init();
    void update();

private:
    Screen _screen;
    ConfScreen _confScreen;

    Screen *_current;
};
