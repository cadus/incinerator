#include "interactive_screen.h"

InteractiveItem::InteractiveItem(InteractiveScreen& parent,
                                 uint16_t x, uint16_t y, uint16_t w, uint16_t h)
:_parent(parent)
,_x(x)
,_y(y)
,_w(w)
,_h(h)
{
}

PushButton::PushButton(InteractiveScreen& parent,
                       std::string text,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h)
:InteractiveItem(parent,x,y,w,h)
,_text(text)
{
    
}

void PushButton::draw(bool selected)
{
    Screen::PrintFlags flags;
    if (selected) {
        flags.set(Screen::PrintFlag::invert);
    }

    _parent.print(_text, _x, _y, _w, _h, flags);
}

bool PushButton::rotate(int digits)
{
    printf("%s rotated by: %d\r\n", _text.c_str(), digits);
    return false;
}

bool PushButton::click()
{
    _parent._fixedItemSelection = !_parent._fixedItemSelection;
    if (_parent._fixedItemSelection) {
        printf("%s selection begin\r\n", _text.c_str());
    } else  {
        printf("%s selection end\r\n", _text.c_str());
    }
    return true;
}

void InteractiveScreen::reset()
{
    constexpr uint16_t y = _content_y;
    constexpr uint16_t dy = 24;

    static PushButton b1(*this, "Button1", 50, y+dy*0, 150, dy);
    static PushButton b2(*this, "Button2", 50, y+dy*1, 150, dy);
    static PushButton b3(*this, "Button3", 50, y+dy*2, 150, dy);

    _items = { &b1, &b2, &b3 };
}

void InteractiveScreen::draw()
{
    ssize_t k = 0;
    for (auto i : _items) {
        i->draw(k++ == _selectedItem);
    }
}

bool InteractiveScreen::handleEncoderRotation(int delta)
{
    const size_t range = _items.size();
    if (!range) {
        // handle edge case of empty item list
        return false;
    }

    if (_selectedItem < 0 || _selectedItem >= range) {
        printf("InteractiveScreen::handleEncoderRotation() index out of range (%d)\r\n", _selectedItem);
    }

    if (_fixedItemSelection) {
        // selected item receives encoder rotation
        return _items[_selectedItem]->rotate(delta);
    }

    // change item selection according to encoder rotation
    ssize_t selected_before =_selectedItem;
    _selectedItem += delta;
    while (_selectedItem < 0) {
        _selectedItem += range;
    }
    while (_selectedItem >= range) {
        _selectedItem -= range;
    }
    return _selectedItem != selected_before;
}

bool InteractiveScreen::handleEncoderSwitch()
{
    const size_t range = _items.size();
    if (!range) {
        // handle edge case of empty item list
        return false;
    }

    if (_selectedItem < 0 || _selectedItem >= range) {
        printf("InteractiveScreen::handleEncoderSwitch() index out of range (%d)\r\n", _selectedItem);
    }

    return _items[_selectedItem]->click();
}
