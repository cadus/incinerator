#include "testscreens.h"

void FooScreen::reset()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t y = _content_y + 100;
    constexpr uint16_t dx = 100;
    constexpr uint16_t dy = 24;

    static PushButton b1(*this, "Back",
                         [] () {
                           printf("Back\r\n"); return true;
                         },
                         x+dx*0, y, dx, dy);
    static PushButton b2(*this, "Bar",
                         [this] () {
                             _nextScreen = &barScreen; printf("Bar\r\n"); return true;
                         },
                         x+dx*1, y, dx, dy);
    static PushButton b3(*this, "Test",
                         [this] () {
                             _nextScreen = &testScreen; printf("Test\r\n"); return true;
                         },
                         x+dx*2, y, dx, dy);

    _items = { &b1, &b2, &b3 };

    InteractiveScreen::reset();
}

void BarScreen::reset()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t y = _content_y + 100;
    constexpr uint16_t dx = 100;
    constexpr uint16_t dy = 24;

    static PushButton b1(*this, "Back",
                         [] () {
                           printf("Back\r\n"); return true;
                         },
                         x+dx*0, y, dx, dy);
    static PushButton b2(*this, "Foo",
                         [this] () {
                             _nextScreen = &fooScreen; printf("Foo\r\n"); return true;
                         },
                         x+dx*1, y, dx, dy);
    static PushButton b3(*this, "Test",
                         [this] () {
                             _nextScreen = &testScreen; printf("Test\r\n"); return true;
                         },
                         x+dx*2, y, dx, dy);

    _items = { &b1, &b2, &b3 };

    InteractiveScreen::reset();
}

void TestScreen::reset()
{
    constexpr uint16_t x = 30;
    constexpr uint16_t y = _content_y + 100;
    constexpr uint16_t dx = 100;
    constexpr uint16_t dy = 24;

    static PushButton b1(*this, "Back",
                         [] () {
                           printf("Back\r\n"); return true;
                         },
                         x+dx*0, y, dx, dy);
    static PushButton b2(*this, "Foo",
                         [this] () {
                             _nextScreen = &fooScreen; printf("Foo\r\n"); return true;
                         },
                         x+dx*1, y, dx, dy);
    static PushButton b3(*this, "Bar",
                         [this] () {
                             _nextScreen = &barScreen; printf("Bar\r\n"); return true;
                         },
                         x+dx*2, y, dx, dy);

    _items = { &b1, &b2, &b3 };

    InteractiveScreen::reset();
}

FooScreen fooScreen;
BarScreen barScreen;
TestScreen testScreen;
