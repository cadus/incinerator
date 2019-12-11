#include "ui.h"
#include "buzzer.h"
#include "debounced_encoder.h"

void Ui::init()
{
    Screen::init();
    buzzer.init();
    encoder_init();

    _current = &_confScreen;
    _current->reset();
}

void Ui::task()
{
    if (_to.elapsed() || _updateReq) {
        _updateReq = false;
        _to.set(1000);
        _current->update();
    }
}

void Ui::backgroundTask()
{
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
}
