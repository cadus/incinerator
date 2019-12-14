#include "ui.h"
#include "buzzer.h"
#include "debounced_encoder.h"
#include "screens/conf_screen.h"

Ui::Ui()
:_current(nullptr)
,_screenChange(true)
,_updateReq(false)
,_encoderPos(0)
,_encoderSw(false)
{
}

void Ui::init()
{
    Screen::init();
    buzzer.init();
    encoder_init();

    _current = &confScreen;
    _current->reset();
}

void Ui::task()
{
    if (_to.elapsed() || _updateReq) {
        _updateReq = false;
        _to.set(1000);
        bool fullRefresh = _screenChange;
        // reset _screenChange before updating, b/c it can be set again during the update
        _screenChange = false;
        _current->update(fullRefresh);
    }
}

void Ui::backgroundTask()
{
    if (!_current || _screenChange) {
        return;
    }

    int encoderPos = encoder_position();
    if (encoderPos != _encoderPos) {
        buzzer.buzz(16);
        _updateReq |= _current->handleEncoderRotation(encoderPos - _encoderPos);
        _encoderPos = encoderPos;
    }

    const bool encoderSw = encoder_switch();
    if (encoderSw != _encoderSw) {
        if (encoderSw) {
            buzzer.buzz(50);
            _updateReq |= _current->handleEncoderSwitch();
        }
        _encoderSw = encoderSw;
    }
    auto next = _current->getNextScreen();
    if (next) {
        // Screen has changed
        _current->setNextScreen(nullptr);
        _current = next;
        _current->reset();
        _updateReq |= true;
        _screenChange = true;
    }
}
