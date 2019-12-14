#pragma once

#include "ui/screens/screen.h"
#include "ui/screens/conf_screen.h"
#include "ui/screens/interactive_screen.h"

#include "util/timeout.h"

class Ui
{
public:
    void init();
    void task();
    void backgroundTask();
    void switchScreen(Screen *newScreen);

private:
    ConfScreen _confScreen;
    InteractiveScreen _interactiveScreen;

    Screen *_current;

    Timeout _to;
    bool _updateReq;
    int _encoderPos;
    bool _encoderSw;
};
