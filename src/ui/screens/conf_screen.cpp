#include "conf_screen.h"

void ConfScreen::draw()
{
    _d.drawLine(0, _content_y, 399, _content_y + _content_height, GxEPD_BLACK);
    _d.drawLine(399, _content_y, 0, _content_y + _content_height, GxEPD_BLACK);
}
