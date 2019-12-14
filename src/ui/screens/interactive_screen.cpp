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
                       std::function<bool()> handler,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h)
:InteractiveItem(parent,x,y,w,h)
,_text(text)
,_handler(handler)
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
    // push button doesn't accept encoder input
    return false;
}

bool PushButton::click()
{
    return _handler();
}

ValueEntry::ValueEntry(InteractiveScreen& parent,
                       std::string text,
                       std::string unit,
                       int initial,
                       int step,
                       int lowerBound,
                       int upperBound,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h)
:InteractiveItem(parent, x, y, w, h)
,_text(text)
,_unit(unit)
,_val(initial)
,_step(step)
,_lowerBound(lowerBound)
,_upperBound(upperBound)
{
}

void ValueEntry::draw(bool selected)
{
    uint16_t x = _x;

    Screen::PrintFlags flags;
    flags.set(Screen::PrintFlag::justifyRight);
    if (selected && !_parent._fixedItemSelection) {
        flags.set(Screen::PrintFlag::invert);
    }
    Screen::print(_text, x, _y, _w / 2 - 4, _h, flags);

    x += _w / 2 + 4;

    flags = 0;
    flags.set(Screen::PrintFlag::justifyLeft);
    if (selected && _parent._fixedItemSelection) {
        flags.set(Screen::PrintFlag::invert);
    }
    char tmp[80];
    snprintf(tmp, sizeof(tmp), "%d%s", _val, _unit.c_str());
    Screen::print(tmp, x, _y, _w / 2 - 4, _h, flags);
}

bool ValueEntry::rotate(int digits)
{
    auto val_before = _val;
    _val += digits * _step;
    _val = min(_upperBound, _val);
    _val = max(_lowerBound, _val);

    return val_before != _val;
}

bool ValueEntry::click()
{
    if (_parent._fixedItemSelection) {
        printf("%s update to %d\r\n", _text.c_str(), _val);
    }
    _parent._fixedItemSelection = !_parent._fixedItemSelection;
    return true;
}

void InteractiveScreen::reset()
{
    _selectedItem = 0;
    _fixedItemSelection = false;
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
        // shouldn't happen
        return false;
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
        // shouldn't happen
        return false;
    }

    return _items[_selectedItem]->click();
}
