#include "MidiSequencer.h"

MidiSequencer::MidiSequencer(QObject *parent) : QThread(parent)
{
    _eTimer = new QElapsedTimer();
}

MidiSequencer::~MidiSequencer()
{
    delete _eTimer;
}

void MidiSequencer::setMidi(MidiFile *midi)
{
    this->stop();
    this->_midi = midi;
}

void MidiSequencer::stop(bool resetPos)
{
    if (_stopped)
        return;

    _playing = false;
    _stopped = false;
    _startPlayIndex = _playedIndex;

    while (!isFinished()) {};

    if (resetPos) {
        _stopped = true;
        _startPlayTime = 0;
        _startPlayIndex = 0;
        _playedIndex = 0;
        _positionMs = 0;
        _positionTick = 0;
    }
}

void MidiSequencer::run()
{
    if (_playing)
        return;

    _playing = true;
    _stopped = false;


    if (_playedIndex > 0) {
        uint32_t tick = _midi->events()[_playedIndex]->tick();
        _startPlayTime = _midi->timeFromTick(tick, _midiSpeed) * 1000;
    }

    _eTimer->restart();

    for (int i = 0; i < _midi->events().count(); i++) {

        if (!_playing)
            break;

        if (_midi->events()[i]->eventType() != MidiEventType::Meta) {

            uint32_t tick = _midi->events()[i]->tick();

            if (_midiChangeBpmSpeed) {
                _midiChangeBpmSpeed = false;
                _midiSpeed = _midiSpeedTemp;
                _startPlayTime = _midi->timeFromTick(_midi->events()[i-1]->tick(), _midiSpeed) * 1000;
                _eTimer->restart();
            }

            long eventTime = (_midi->timeFromTick(tick, _midiSpeed)  * 1000);
            long waitTime = eventTime - _startPlayTime  - _eTimer->elapsed();

            if (waitTime > 0) {
                msleep(waitTime);
            }

            if (_midi->events()[i]->eventType() != MidiEventType::SysEx) {
                emit playingEvent(_midi->events()[i]);
            }

            _positionMs = eventTime;

        } else { // Meta event

        }

        _playedIndex = i;
        _positionTick = _midi->events()[i]->tick();

    } // End for loop
}
