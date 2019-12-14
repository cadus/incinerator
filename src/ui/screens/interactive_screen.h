#pragma once

#include <functional>
#include <string>

#include "screen.h"

class InteractiveScreen;

class InteractiveItem
{
public:
    InteractiveItem(InteractiveScreen& parent,
                    uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    virtual void draw(bool selected) = 0;
    virtual bool rotate(int digits) = 0;
    virtual bool click() = 0;

    InteractiveScreen& _parent;
    uint16_t _x;
    uint16_t _y;
    uint16_t _w;
    uint16_t _h;
};

class PushButton : public InteractiveItem
{
public:
    PushButton(InteractiveScreen& parent,
               std::string text,
               std::function<bool()> handler,
               uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    virtual void draw(bool selected) override;
    virtual bool rotate(int digits) override;
    virtual bool click() override;

private:
    std::string _text;
    std::function<bool()> _handler;
};

class ScreenChangeButton : public PushButton
{
public:
    ScreenChangeButton(InteractiveScreen& parent,
                       std::string text,
                       Screen *target,
                       uint16_t x, uint16_t y, uint16_t w, uint16_t h);
};

class ValueEntry : public InteractiveItem
{
public:
    ValueEntry(InteractiveScreen& parent,
               std::string text,
               std::string unit,
               int initial,
               int step,
               int lowerBound,
               int upperBound,
               uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    virtual void draw(bool selected) override;
    virtual bool rotate(int digits) override;
    virtual bool click() override;

    virtual void update() = 0;

    std::string _text;
    std::string _unit;
    int _val;
    int _step;
    int _lowerBound;
    int _upperBound;
};

class InteractiveScreen : public Screen
{
    friend class PushButton;
    friend class ScreenChangeButton;
    friend class ValueEntry;

public:
    InteractiveScreen(bool wraparound = false);

    void reset() override;
    void draw() override;
    bool handleEncoderRotation(int delta) override;
    bool handleEncoderSwitch() override;

protected:
    std::vector<InteractiveItem*> _items;
    ssize_t _selectedItem;
    bool _fixedItemSelection;
    bool _wraparound;
};
