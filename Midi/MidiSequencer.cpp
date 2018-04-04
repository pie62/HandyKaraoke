#include "MidiSequencer.h"

MidiSequencer::MidiSequencer(QObject *parent) : QThread(parent)
{
    _midi = new MidiFile();
    _eTimer = new QElapsedTimer();
}

MidiSequencer::~MidiSequencer()
{
    stop();

    delete _eTimer;
    delete _midi;
}

int MidiSequencer::beatCount()
{
    if (_midi->events().count() == 0) {
        return 0;
    } else {
        return _midi->beatFromTick(_midi->events().back()->tick());
    }
}

int MidiSequencer::currentBeat()
{
    return _midi->beatFromTick(positionTick());
}

int MidiSequencer::positionTick()
{
    if (_playing) {
        return _midi->tickFromTimeMs(_eTimer->elapsed()  + _startPlayTime, _midiSpeed);
    } else {
        return _positionTick;
    }
}

int MidiSequencer::durationTick()
{
    return _midi->events().back()->tick();
}

long MidiSequencer::positionMs()
{
    return _playing ? _midi->timeFromTick(positionTick()) * 1000 : _positionMs;
}

long MidiSequencer::durationMs()
{
    return _midi->timeFromTick(durationTick()) * 1000;
}

void MidiSequencer::setPositionTick(int t)
{
    bool playAfterSeek = _playing;
    if (_playing)
        stop();

    int index = 0;
    for (MidiEvent *e :_midi->events()) {
        if (e->tick() > t)
            break;

        if (e->eventType() == MidiEventType::Controller
            || e->eventType() == MidiEventType::ProgramChange) {
            emit playingEvent(e);
        }
        _positionTick = e->tick();
        index++;
    }

    _playedIndex = index;
    if (playAfterSeek)
        start();
}

void MidiSequencer::setBpmSpeed(int sp)
{
    if (sp == _midiSpeed)
        return;

    if ((_midiBpm + sp) < 20 || (_midiBpm + sp) > 250)
        return;

    if (_playing) {
        _midiSpeedTemp = sp;
        _midiChangeBpmSpeed = true;
    } else {
        _midiSpeed = sp;
    }

    emit bpmChanged(_midiBpm + sp);
}

bool MidiSequencer::load(const QString &file, bool seekFileChunkID)
{
    if (!_stopped)
        stop();

    if (!_midi->read(file, seekFileChunkID))
        return false;

    _midiSpeed = 0;
    _midiSpeedTemp = 0;
    _midiChangeBpmSpeed = false;

    _finished = false;

    if (_midi->tempoEvents().count() > 0) {
        _midiBpm = _midi->tempoEvents()[0]->bpm();
    } else {
        _midiBpm = 120;
    }

    return true;
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
    _finished = false;


    if (_playedIndex > 0) {
        uint32_t tick = _midi->events()[_playedIndex]->tick();
        _startPlayTime = _midi->timeFromTick(tick, _midiSpeed) * 1000;
    }

    _eTimer->restart();

    for (int i = _playedIndex; i < _midi->events().count(); i++) {

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

            _positionMs = eventTime;

        } else { // Meta event
            if (_midi->events()[i]->metaEventType() == MidiMetaType::SetTempo) {
                _midiBpm = _midi->events()[i]->bpm();
                emit bpmChanged(_midiBpm + _midiSpeed);
            }
        }

        emit playingEvent(_midi->events()[i]);

        _playedIndex = i;
        _positionTick = _midi->events()[i]->tick();

    } // End for loop

    if (_playedIndex == _midi->events().size() -1 ) {
        _finished = true;
    }
}
