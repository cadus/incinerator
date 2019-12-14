#pragma once

#include "ui/screens/screen.h"
#include "util/timeout.h"

class Ui
{
public:
    void init();
    void task();
    void backgroundTask();
    void switchScreen(Screen *newScreen);

private:
    Screen *_current;

    Timeout _to;
    bool _updateReq;
    int _encoderPos;
    bool _encoderSw;
};
