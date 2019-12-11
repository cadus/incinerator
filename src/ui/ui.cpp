#include "ui.h"

void Ui::init()
{
    _screen.init();
    _current = &_confScreen;
}

void Ui::update()
{
    _current->update();
}
