#include "MidiSequencer.h"

#include <QDebug>

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
        return _midi->beatFromTick(durationTick());
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

    uint32_t tick = 0;
    if (t < _startTick)
        tick = _startTick;
    else if ((_endTick > 0) && (t > _endTick))
        tick = _endTick;
    else if (t > durationTick())
        tick = durationTick();
    else
        tick = t;

    _mutex.lock();

    _playedIndex = eventIndexFromTick(tick);
    _startPlayTime = _midi->timeFromTick(tick, _midiSpeed) * 1000;
    _positionMs = _startPlayTime;
    _positionTick = t;

    _mutex.unlock();

    if (playAfterSeek)
        start();
}

void MidiSequencer::setBpmSpeed(int sp)
{
    if (sp == _midiSpeed)
        return;

    if ((_midiBpm + sp) < 20 || (_midiBpm + sp) > 250)
        return;

    _mutex.lock();

    if (_playing) {
        _midiSpeedTemp = sp;
        _midiChangeBpmSpeed = true;
    } else {
        _midiSpeed = sp;
    }

    _mutex.unlock();

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
    _endTick = _midi->events().last()->tick();

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

    _mutex.lock();
    _playing = false;
    _stopped = false;
    _startPlayIndex = _playedIndex;
    _startPlayTime = positionMs();

    _waitCondition.wakeAll();
    _mutex.unlock();

    wait();

    if (resetPos) {
        _mutex.lock();
        _stopped = true;
        _startPlayTime = _midi->timeFromTick(_startTick, _midiSpeed) * 1000;;
        _startPlayIndex = eventIndexFromTick(_startTick);
        _playedIndex = _startPlayIndex;
        _positionMs = _startPlayTime;
        _positionTick = _startTick;
        _mutex.unlock();
    }
}

void MidiSequencer::setStartTick(int tick)
{
    _startTick = tick;

    if (_stopped) {
        _mutex.lock();
        _startPlayTime = _midi->timeFromTick(tick, _midiSpeed) * 1000;
        _startPlayIndex = eventIndexFromTick(tick);
        _playedIndex = _startPlayIndex;
        _positionMs = _startPlayTime;
        _positionTick = tick;
        _mutex.unlock();
    }
}

void MidiSequencer::setEndTick(int tick)
{
    if (tick == 0) {
        _endTick = 0;
        if (_endEvent != nullptr) {
            _midi->takeEvent(_endEventIndex);
            delete _endEvent;
            _endEvent = nullptr;
            _endEventIndex = 0;
        }
    } else if (tick > _startTick) {
        _endTick = tick;
        if (_endEvent == nullptr) {
            _endEvent = new MidiEvent();
        } else {
            _midi->takeEvent(_endEventIndex);
        }
        _endEvent->setTick(tick);

        MidiEvent *halfEvent = _midi->events()[ _midi->events().size() / 2 ];

        int index;

        if (tick >= halfEvent->tick()) {
            index = _midi->events().size() - 1;
            for (int i = index; i >= 0; i--) {
                index = i;
                if (_midi->events()[i]->tick() < tick)
                    break;
            }
        } else {
            index = 0;
            for (int i = index; i < _midi->events().size(); i++) {
                index = i;
                if (_midi->events()[i]->tick() >= tick)
                    break;
            }
        }

        _endEventIndex = index;
        _midi->insertEvent(_endEventIndex, _endEvent);
    }
}

void MidiSequencer::setCutStartBar(int bar)
{
    setStartTick(_midi->tickFromBar(bar));
}

void MidiSequencer::setCutEndBar(int bar)
{
    int endbar = (_midi->barCount() + 1) - bar;
    if (bar <= 0)
        endbar--;

    setEndTick(_midi->tickFromBar(endbar));
}

int MidiSequencer::currentBar()
{
    return _midi->barFromTick(positionTick());
}

void MidiSequencer::run()
{
    if (_playing)
        return;

    _mutex.lock();

    _playing = true;
    _stopped = false;
    _finished = false;

    _mutex.unlock();

    if (_positionTick <= _startTick) {
        for (MidiEvent *e : _midi->events()) {
            if (e->tick() >= _startTick)
                break;
            if (e->eventType() != MidiEventType::Meta)
                switch (e->eventType()) {
                case MidiEventType::NoteOff:
                case MidiEventType::NoteOn:
                    break;
                default:
                    emit playingEvent(*e);
                }
            else if (e->metaEventType() == MidiMetaType::SetTempo) {
                if (_midi->isSingleTempo() && (e != _midi->tempoEvents()[0]))
                    continue;
                _midiBpm = e->bpm();
                emit bpmChanged(_midiBpm + _midiSpeed);
            }
        }
    }

    bool seqEnded = false;

    _eTimer->restart();

    for (int i = _playedIndex; i < _midi->events().count(); i++) {

        if (!_playing)
            break;

        if ((_endEvent != nullptr) && (_endTick > 0) && _midi->events()[i]->tick() >= _endEvent->tick()) {
            MidiEvent evt;
            evt.setEventType(MidiEventType::Controller);
            evt.setData1(123);
            evt.setData2(0);
            for (int ch = 0; ch < 16; ch++) {
                evt.setChannel(ch);
                emit playingEvent(evt);
            }
            seqEnded = true;
            break;
        }

        _mutex.lock();

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
                //msleep(waitTime);
                _waitCondition.wait(&_mutex, waitTime);
            }

            _positionMs = eventTime;


        } else { // Meta event
            if (_midi->events()[i]->metaEventType() == MidiMetaType::SetTempo) {
                if (_midi->isSingleTempo() && (_midi->events()[i] != _midi->tempoEvents()[0])) {
                    _playedIndex = i;
                    _positionTick = _midi->events()[i]->tick();
                    _mutex.unlock();
                    continue;
                }
                _midiBpm = _midi->events()[i]->bpm();
                emit bpmChanged(_midiBpm + _midiSpeed);
            }
        }

        emit playingEvent(*_midi->events()[i]);

        _playedIndex = i;
        _positionTick = _midi->events()[i]->tick();

        _mutex.unlock();

    } // End for loop

    if (seqEnded || (_playedIndex == _midi->events().size() -1)) {
        _mutex.lock();
        _finished = true;
        _mutex.unlock();
    }
}

int MidiSequencer::eventIndexFromTick(int tick)
{
    int index = 0;
    for (MidiEvent *e :_midi->events()) {
        if (e->tick() >= tick)
            break;

        index++;
    }

    return index;
}
