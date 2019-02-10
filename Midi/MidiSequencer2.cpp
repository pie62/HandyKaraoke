#include "MidiSequencer2.h"

MidiSequencer2::MidiSequencer2(QObject *parent) : QObject(parent)
{
    _midi = new MidiFile2();

    _timer = new QTimer();
    _timer->setTimerType(Qt::PreciseTimer);

    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerTick()));
}

MidiSequencer2::~MidiSequencer2()
{
    stop();

    delete _timer;
    delete _midi;
}

int MidiSequencer2::beatCount()
{
    if (_midi->tracks().count() == 0) {
        return 0;
    } else {
        return _midi->beatFromTick(durationTick());
    }
}

int MidiSequencer2::currentBeat()
{
    return _midi->beatFromTick(_positionTick);
}

int MidiSequencer2::positionTick()
{
    return _positionTick;
}

int MidiSequencer2::durationTick()
{
    int tick = 0;

    for (auto evts : _midi->tracks())
    {
        if (evts.back()->tick() > tick) {
            tick = evts.back()->tick();
        }
    }

    return tick;
}

long MidiSequencer2::positionMs()
{
    return _midi->timeFromTick(positionTick()) * 1000;
}

long MidiSequencer2::durationMs()
{
    return _midi->timeFromTick(durationTick()) * 1000;
}

void MidiSequencer2::setPositionTick(int t)
{

}

void MidiSequencer2::setBpmSpeed(int sp)
{

}

bool MidiSequencer2::load(const QString &file, bool seekFileChunkID)
{
    if (!_stopped)
        stop();

    if (!_midi->read(file, seekFileChunkID))
        return false;

    _midiSpeed = 0;

    _finished = false;

    if (_midi->tempoEvents().count() > 0) {
        _midiBpm = _midi->tempoEvents()[0]->bpm();
    } else {
        _midiBpm = 120;
    }

    _playedIndexs.clear();
    for (int t=0; t<_midi->numberOfTracks(); t++) {
        _playedIndexs.append(0);
    }

    _timer->setInterval(calcIntervalTime());

    return true;
}

void MidiSequencer2::start()
{
    if (_playing)
        return;

    _playing = true;
    _stopped = false;
    _finished = false;

    _timer->start();
}

void MidiSequencer2::stop(bool resetPos)
{
    if (_stopped)
        return;

    _timer->stop();

    _playing = false;
    _stopped = false;

    if (resetPos)
    {
        _stopped = true;
        _positionTick = 0;

        for (int t=0; t<_midi->numberOfTracks(); t++)
            _playedIndexs[t] = 0;
    }
}

void MidiSequencer2::onTimerTick()
{
    for (int t = 0; t < _midi->numberOfTracks(); t++)
    {
        for (int idx = _playedIndexs[t]; idx < _midi->tracks()[t].count(); idx++)
        {
            if (!_playing)
                break;

            MidiEvent *e = _midi->tracks()[t][idx];

            if (e->tick() > _positionTick)
                break;

            _playedIndexs[t] = idx;

            if(e->eventType() == MidiEventType::Meta)
            {
                if (e->metaEventType() == MidiMetaType::SetTempo) {
                    _midiBpm = e->bpm();
                    _timer->setInterval(calcIntervalTime());
                    emit bpmChanged(_midiBpm + _midiSpeed);
                }
            }

            emit playingEvent(e);

        } // loop events
    } // loop tracks

    _positionTick += _midi->resorution() / 8;

    if (_positionTick >= durationTick()) {
        _finished = true;
    }
}

float MidiSequencer2::calcIntervalTime()
{
    float timePerTick = 60000 / ((_midiBpm + _midiSpeed) * _midi->resorution());

    return (_midi->resorution() / 8) * timePerTick;
}
