#include "testscreens.h"

void FooScreen::reset()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t y = _content_y + 100;
    constexpr uint16_t dx = 100;
    constexpr uint16_t dy = 24;

    static ScreenChangeButton b1(*this, "Bar", &barScreen, x+dx*0, y, dx, dy);
    static ScreenChangeButton b2(*this, "Test", &testScreen, x+dx*1, y, dx, dy);
    _items = { &b1, &b2 };

    InteractiveScreen::reset();
}

void BarScreen::reset()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t y = _content_y + 100;
    constexpr uint16_t dx = 100;
    constexpr uint16_t dy = 24;

    static ScreenChangeButton b1(*this, "Foo", &fooScreen, x+dx*0, y, dx, dy);
    static ScreenChangeButton b2(*this, "Test", &testScreen, x+dx*1, y, dx, dy);
    _items = { &b1, &b2 };

    InteractiveScreen::reset();
}

void TestScreen::reset()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t y = _content_y + 100;
    constexpr uint16_t dx = 100;
    constexpr uint16_t dy = 24;

    static ScreenChangeButton b1(*this, "Foo", &fooScreen, x+dx*0, y, dx, dy);
    static ScreenChangeButton b2(*this, "Bar", &barScreen, x+dx*1, y, dx, dy);
    _items = { &b1, &b2 };

    InteractiveScreen::reset();
}

FooScreen fooScreen;
BarScreen barScreen;
TestScreen testScreen;
