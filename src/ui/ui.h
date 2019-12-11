#pragma once

#include "ui/screens/screen.h"
#include "ui/screens/conf_screen.h"

#include "util/timeout.h"

class Ui
{
public:
    void init();
    void task();
    void backgroundTask();

private:
    ConfScreen _confScreen;

    Screen *_current;

    Timeout _to;
    bool _updateReq;
    int _encoderPos;
    bool _encoderSw;
};
