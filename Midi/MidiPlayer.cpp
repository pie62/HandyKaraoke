#include "MidiPlayer.h"

#include <QTimer>
#include <QDebug>

MidiPlayer::MidiPlayer(QObject *parent) : QThread(parent)
{
    _midi       = new MidiFile();
    _midiOut    = new MidiOut();
    _midiSynth  = new MidiSynthesizer();
    _eTimer     = new QElapsedTimer();
}

MidiPlayer::~MidiPlayer()
{
    delete _eTimer;
    delete _midiSynth;
    delete _midiOut;
    delete _midi;
}

std::vector<std::__cxx11::string> MidiPlayer::midiDevices()
{
    std::vector<std::string> outName;
    MidiOut o;
    for (int i=0; i<o.getPortCount(); i++) {
        outName.push_back(o.getPortName(i));
    }
    return outName;
}

bool MidiPlayer::setMidiOut(int portNumer)
{
    if (portNumer != -1 && portNumer >= _midiOut->getPortCount())
        return false;

    if (!_stopped)
        stop();

    bool result = false;
    if (_midiOut->isPortOpen())
        _midiOut->closePort();

    if (portNumer == -1) {
        _midiSynth->setVolume(_volume / 100.0f);
        _midiPortNum = -1;
        result = true;
    } else {
        _midiOut->openPort(portNumer);
        _midiOut->setVolume(_volume / 100.0f);
        result = _midiOut->isPortOpen();
        _midiPortNum = result ? portNumer : _midiPortNum;
    }

    for (int i=0; i<16; i++)
        _midiChannels[i].setPort(_midiPortNum);

    return result;
}

bool MidiPlayer::load(std::__cxx11::string file)
{
    if (!_stopped)
        stop();

    if (!_midi->read(file))
        return false;

    MidiEvent *e = _midi->events().back();
    _durationTick = e->tick();
    _durationMs = _midi->timeFromTick(e->tick()) * 1000;

    _finished = false;

    return true;
}

void MidiPlayer::stop(bool resetPos)
{
    if (_stopped)
        return;

    _playing = false;
    _stopped = false;
    _finished = false;

    while (!isFinished()) {};

    if (resetPos) {
        _stopped = true;
        _startPlayTime = 0;
        _playedIndex = 0;
        _positionMs = 0;
        _positionTick = 0;
    }
}

void MidiPlayer::setVolume(int v)
{
    if (v < 0) _volume = 0;
    else if (v > 100) _volume = 100;
    else _volume = v;

    _midiOut->setVolume(_volume / 100.0f);
    _midiSynth->setVolume(_volume / 100.0f);
}

void MidiPlayer::setPositionTick(int t)
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
            sendEvent(e);
        }

        index++;
    }

    _playedIndex = index;
    if (playAfterSeek)
        start();
}

int MidiPlayer::positionTick()
{
    if (_playing) {
        //float time = (_eTimer->elapsedMs() + _startPlayTime) / 1000;
        float time = _eTimer->elapsed()  + _startPlayTime;
        return _midi->tickFromTimeMs(time);
    } else {
        return _positionTick;
    }
}

void MidiPlayer::run()
{
    if (_playing)
        return;

    if (_stopped)
        sendResetAllControllers();

    _playing = true;
    _stopped = false;
    _finished = false;

    playEvents();
}

long MidiPlayer::positionMs()
{
    return _playing ? _eTimer->elapsed() + _startPlayTime : _positionMs;
}

void MidiPlayer::playEvents()
{
    if (_playedIndex > 0) {
        uint32_t ti = _midi->events()[_playedIndex]->tick();
        _startPlayTime = _midi->timeFromTick(ti) * 1000;
    }

    _eTimer->restart();

    for (int i = _playedIndex; i < _midi->events().size(); i++) {

        if (!_playing)
            break;

        MidiEvent e = *_midi->events()[i];
        if (e.eventType() != MidiEventType::Meta) {

            long eventTime = _midi->timeFromTick(e.tick()) * 1000;
            long waitTime = eventTime - _startPlayTime - _eTimer->elapsed();

            if (waitTime > 0) {
                //std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
                msleep(waitTime);
            }

            if (e.eventType() != MidiEventType::SysEx) {

                if (e.eventType() == MidiEventType::Controller
                    || e.eventType() == MidiEventType::ProgramChange) {
                    sendEvent(&e);
                } else {
                    if (_midiChannels[e.channel()].isMute() == false) {
                        if (_useSolo) {
                            if (_midiChannels[e.channel()].isSolo()) {
                                sendEvent(&e);
                            }
                        } else {
                            sendEvent(&e);
                        }
                    }
                }

            }

            _positionMs = eventTime;

        } else { // Meta event
            if (e.metaEventType() == MidiMetaType::SetTempo) {
                _midiBpm = e.tempoBpm();
            }
        }

        _playedIndex = i;
        _positionTick = e.tick();

        emit playingEvents(&e);

    } // End for loop

    sendAllNotesOff();

    // Check finished
    if (_playedIndex == _midi->events().size() -1 ) {
        _finished = true;
    }
}

void MidiPlayer::sendEvent(MidiEvent *e)
{
    int ch = e->channel();

    switch (e->eventType()) {
    case MidiEventType::NoteOff: {
        if (_midiPortNum == -1) {
            _midiSynth->sendNoteOff(ch, e->data1(), e->data2());
        } else {
            _midiOut->sendNoteOff(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::NoteOn: {
        if (_midiPortNum == -1) {
            _midiSynth->sendNoteOn(ch, e->data1(), e->data2());
        } else {
            _midiOut->sendNoteOn(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::NoteAftertouch: {
        if (_midiPortNum == -1) {
            _midiSynth->sendNoteAftertouch(ch, e->data1(), e->data2());
        } else {
            _midiOut->sendNoteAftertouch(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::Controller: {
        if (e->data1() == 7) _midiChannels[ch].setVolume(e->data2());
        if (e->data1() == 10) _midiChannels[ch].setPan(e->data2());

        if (_midiPortNum == -1) {
            _midiSynth->sendController(ch, e->data1(), e->data2());
        } else {
            _midiOut->sendController(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::ProgramChange: {
        _midiChannels[ch].setInstrument(e->data1());
        if (_midiPortNum == -1) {
            _midiSynth->sendProgramChange(ch, e->data1());
        } else {
            _midiOut->sendProgramChange(ch, e->data1());
        }
        break;
    }
    case MidiEventType::ChannelAftertouch: {
        if (_midiPortNum == -1) {
            _midiSynth->sendChannelAftertouch(ch, e->data1());
        } else {
            _midiOut->sendChannelAftertouch(ch, e->data1());
        }
        break;
    }
    case MidiEventType::PitchBend: {
        if (_midiPortNum == -1) {
            _midiSynth->sendPitchBend(ch, e->data1());
        } else {
            _midiOut->sendPitchBend(ch, e->data1());
        }
        break;
    }
    }
}

void MidiPlayer::sendAllNotesOff(int ch)
{
    if (_midiPortNum == -1) {
        _midiSynth->sendAllNotesOff(ch);
    } else {
        _midiOut->sendAllNotesOff(ch);
    }
}

void MidiPlayer::sendAllNotesOff()
{
    if (_midiPortNum == -1) {
        _midiSynth->sendAllNotesOff();
    } else {
        _midiOut->sendAllNotesOff();
    }
}

void MidiPlayer::sendResetAllControllers()
{
    if (_midiPortNum == -1) {
        for (int i=0; i<16; i++) {
            _midiSynth->sendController(i, 121, 0);
        }
    } else {
        for (int i=0; i<16; i++) {
            _midiOut->sendController(i, 121, 0);
        }
    }
}
