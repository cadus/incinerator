#include "testscreens.h"

void FooScreen::reset()
{
    constexpr uint16_t y = _content_y;
    constexpr uint16_t dy = 24;

    static PushButton b1(*this, "Button1", 50, y+dy*0, 150, dy);
    static ValueEntry v1(*this, "Value1", "ms", 42, 1, 0, 100,
                         50, y+dy*1, 150, dy);
    static PushButton b2(*this, "Button2", 50, y+dy*2, 150, dy);
    static ValueEntry v2(*this, "Value2", "ms", 42, 1, 0, 100,
                         50, y+dy*3, 150, dy);
    static PushButton b3(*this, "Button3", 50, y+dy*4, 150, dy);

    _items = { &b1, &v1, &b2, &v2, &b3 };

    InteractiveScreen::reset();
}
