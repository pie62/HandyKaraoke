#include "MidiPlayer.h"

#include <QTimer>
#include <QtMath>
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

std::vector<std::string> MidiPlayer::midiDevices()
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
        _midiSynth->open();
        _midiSynth->setVolume(_volume / 100.0f);
        _midiPortNum = -1;
        result = true;
    } else {
        _midiSynth->close();
        _midiOut->openPort(portNumer);
        _midiOut->setVolume(_volume / 100.0f);
        result = _midiOut->isPortOpen();
        _midiPortNum = result ? portNumer : _midiPortNum;
    }

    for (int i=0; i<16; i++)
        _midiChannels[i].setPort(_midiPortNum);

    return result;
}

bool MidiPlayer::load(std::string file, bool seekFileChunkID)
{
    if (!_stopped)
        stop();

    if (!_midi->read(file, seekFileChunkID))
        return false;

    MidiEvent *e = _midi->events().back();
    _durationTick = e->tick();
    _durationMs = _midi->timeFromTick(e->tick()) * 1000;
    _midiTranspose = 0;

    tempo_scale = 100;

    _finished = false;

    { // Calculate beat count
        uint32_t t = _midi->events().back()->tick();
        _midiBeatCount = _midi->beatFromTick(t);

        _beatInBar.clear();
        int beatCalculed = 0;
        int nBeatInBar = 0;
        for (MidiEvent *evt : _midi->timeSignatureEvents()) {
            if (nBeatInBar > 0) {
                int nBeat = _midi->beatFromTick(evt->tick()) - beatCalculed;
                int nBar = nBeat / nBeatInBar;
                beatCalculed += nBeat;
                _beatInBar.insertMulti(nBeatInBar, nBar);
            }
            nBeatInBar = getNumberBeatInBar(evt->data()[0], evt->data()[1]);
        }
        int nBeat = _midiBeatCount - beatCalculed;
        int nBar = nBeat / nBeatInBar;
        _beatInBar.insertMulti(nBeatInBar, nBar);
    } // End calculate beat count

    for (int i=0; i<16; i++) {
        _midiChannels[i].setInstrument(0);
        _midiChannels[i].setVolume(100);
        _midiChannels[i].setPan(64);
        _midiChannels[i].setReverb(0);
        _midiChannels[i].setChorus(0);
        _midiChannels[i].setInstrumentType(InstrumentType::Piano);
    }
    _midiChannels[9].setInstrumentType(InstrumentType::PercussionEtc);
    emit loaded();

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

void MidiPlayer::setVolume(int ch, int v)
{
    if (ch < 0 || ch > 15)
        return;

    int vl = 0;
    if (v > 127) vl = 127;
    else if (v < 0) vl = 0;
    else vl = v;

    MidiEvent evt;
    evt.setChannel(ch);
    evt.setEventType(MidiEventType::Controller);
    evt.setData1(7);
    evt.setData2(vl);

    sendEvent(&evt);
}

void MidiPlayer::setInstrument(int ch, int i)
{
    if (ch < 0 || ch > 15)
        return;

    int v = 0;
    if (i > 127) v = 127;
    else if (i < 0) v = 0;
    else v = i;

    /*MidiEvent evt;
    evt.setChannel(ch);
    evt.setEventType(MidiEventType::ProgramChange);
    evt.setData1(v);

    sendEvent(&evt);*/
    if (_midiPortNum == -1) {
        _midiSynth->sendProgramChange(ch, v);
    } else {
        _midiOut->sendProgramChange(ch, v);
    }
    _midiChannels[ch].setInstrument(v);
    _midiChannels[ch].setInstrumentType(MidiHelper::getInstrumentType(v));
}

void MidiPlayer::setMute(int ch, bool mute)
{
    if (ch < 0 || ch > 15)
        return;

    if (mute == _midiChannels[ch].isMute())
        return;

    _midiChannels[ch].setMute(mute);
    if (mute)
        sendAllNotesOff(ch);
}

void MidiPlayer::setSolo(int ch, bool solo)
{
    if (ch < 0 || ch > 15)
        return;

    if (solo == _midiChannels[ch].isSolo())
        return;

    _midiChannels[ch].setSolo(solo);

    bool us = false;
    for (int i=0; i<16; i++) {
        if (_midiChannels[i].isSolo())
            us = true;
        else {
            sendAllNotesOff(i);
        }
    }

    _useSolo = us;

    if (_playing) {
        for (int i=0; i<16; i++) {
            if (_midiChannels[i].isSolo())
                continue;
            sendAllNotesOff(i);
        }
    }
}

void MidiPlayer::setPan(int ch, int v)
{
    if (ch < 0 || ch > 15)
        return;

    int vl = 0;
    if (v > 127) vl = 127;
    else if (v < 0) vl = 0;
    else vl = v;

    MidiEvent evt;
    evt.setChannel(ch);
    evt.setEventType(MidiEventType::Controller);
    evt.setData1(10);
    evt.setData2(vl);

    sendEvent(&evt);
}

void MidiPlayer::setReverb(int ch, int v)
{
    if (ch < 0 || ch > 15)
        return;

    int vl = 0;
    if (v > 127) vl = 127;
    else if (v < 0) vl = 0;
    else vl = v;

    MidiEvent evt;
    evt.setChannel(ch);
    evt.setEventType(MidiEventType::Controller);
    evt.setData1(91);
    evt.setData2(vl);

    sendEvent(&evt);
}

void MidiPlayer::setChorus(int ch, int v)
{
    if (ch < 0 || ch > 15)
        return;

    int vl = 0;
    if (v > 127) vl = 127;
    else if (v < 0) vl = 0;
    else vl = v;

    MidiEvent evt;
    evt.setChannel(ch);
    evt.setEventType(MidiEventType::Controller);
    evt.setData1(93);
    evt.setData2(vl);

    sendEvent(&evt);
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
        _positionTick = e->tick();
        index++;
    }

    _playedIndex = index;
    if (playAfterSeek)
        start();
}

void MidiPlayer::setTranspose(int t)
{
    if (_midiTranspose == -12 || _midiTranspose == 12)
        return;

    _midiTranspose = t;

    if (_playing) {
        for (int i=0; i<16; i++) {
            if (i == 9)
                continue;
            sendAllNotesOff(i);
        }
    }
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

int MidiPlayer::currentBeat()
{
    return _midi->beatFromTick(positionTick());
}

bool MidiPlayer::isSnareNumber(int num) {
    if ( (num == 38) || (num == 40) )
        return true;
    else
        return false;
}

bool MidiPlayer::isBassInstrument(int ints) {
    if ( (ints < 32) || (ints > 39) )
        return false;
    else
        return true;
}

void MidiPlayer::setLockDrum(bool lock, int number)
{
    _lockDrum = lock;
    _lockDrumNumber = number;

    if (lock && !_stopped) {
        MidiEvent ev;
        ev.setEventType(MidiEventType::ProgramChange);
        ev.setChannel(9);
        ev.setData1(number);
        sendEvent(&ev);
        emit playingEvents(&ev);
    }
}

void MidiPlayer::setLockSnare(bool lock, int number)
{
    if ( !isSnareNumber(number) )
        return;

    _lockSnare = lock;
    _lockSnareNumber = number;

    if (_playing) {
        sendAllNotesOff(9);
    }
}

void MidiPlayer::setLockBass(bool lock, int number)
{
    if ( !isBassInstrument(number) )
        return;

    _lockBass = lock;
    _lockBassBumber = number;

    if (_stopped)
        return;

    if (lock) {
        for (int i=0; i<16; i++) {
            if (!isBassInstrument(_midiChannels[i].instrument()))
                continue;
            MidiEvent ev;
            ev.setEventType(MidiEventType::ProgramChange);
            ev.setChannel(i);
            ev.setData1(number);
            sendEvent(&ev);
            emit playingEvents(&ev);
        }
    }
}

void MidiPlayer::run()
{
    if (_playing)
        return;

    if (_stopped) {
        sendResetAllControllers();
        MidiEvent ev;
        ev.setEventType(MidiEventType::ProgramChange);
        ev.setChannel(9);
        if (_lockDrum) {
            ev.setData1(_lockDrumNumber);
        } else {
            ev.setData1(0);
        }
        sendEvent(&ev);
        emit playingEvents(&ev);
    }

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

        _playingEventPtr = _midi->events()[i];

        if (_midi->events()[i]->eventType() != MidiEventType::Meta) {

            long eventTime = _midi->timeFromTick(_midi->events()[i]->tick()) * 1000;//* (tempo_scale * 0.01);
            long waitTime = eventTime - _startPlayTime - _eTimer->elapsed();
            if (waitTime > 0) {
                msleep(waitTime);
            }

//            qint32 waitTime;
//            do {
//                waitTime = eventTime - _eTimer->elapsed();
//                if (waitTime > 0) {
//                    msleep(waitTime);
//                }
//            } while (waitTime > 0);

            if (_midi->events()[i]->eventType() != MidiEventType::SysEx) {

//                if (_midi->events()[i]->eventType() == MidiEventType::Controller
//                    || _midi->events()[i]->eventType() == MidiEventType::ProgramChange) {
//                    sendEvent(_midi->events()[i]);
//                } else {
                    if (_midiChannels[_midi->events()[i]->channel()].isMute() == false) {
                        if (_useSolo) {
                            if (_midiChannels[_midi->events()[i]->channel()].isSolo()) {
                                sendEvent(_midi->events()[i]);
                            }
                        } else {
                            sendEvent(_midi->events()[i]);
                        }
                    }
//                }

            }

            _positionMs = eventTime;

        } else { // Meta event
            if (_midi->events()[i]->metaEventType() == MidiMetaType::SetTempo) {
                _midiBpm = _midi->events()[i]->tempoBpm();
                emit bpmChanged(_midiBpm);
            }
        }

        _playedIndex = i;
        _positionTick = _midi->events()[i]->tick();

        emit playingEvents(_playingEventPtr);

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
        int n = getNoteNumberToPlay(ch, e->data1());
        if (_midiPortNum == -1) {
            _midiSynth->sendNoteOff(ch, n, e->data2());
        } else {
            _midiOut->sendNoteOff(ch, n, e->data2());
        }
        break;
    }
    case MidiEventType::NoteOn: {
        int n = getNoteNumberToPlay(ch, e->data1());
        if (_midiPortNum == -1) {
            _midiSynth->sendNoteOn(ch, n, e->data2());
        } else {
            _midiOut->sendNoteOn(ch, n, e->data2());
        }
        break;
    }
    case MidiEventType::NoteAftertouch: {
        int n = getNoteNumberToPlay(ch, e->data1());
        if (_midiPortNum == -1) {
            _midiSynth->sendNoteAftertouch(ch, n, e->data2());
        } else {
            _midiOut->sendNoteAftertouch(ch, n, e->data2());
        }
        break;
    }
    case MidiEventType::Controller: {
        switch (e->data1()) {
        case 7: _midiChannels[ch].setVolume(e->data2()); break;
        case 10: _midiChannels[ch].setPan(e->data2()); break;
        case 91: _midiChannels[ch].setReverb(e->data2()); break;
        case 93: _midiChannels[ch].setChorus(e->data2()); break;
        default: break;
        }

        if (_midiPortNum == -1) {
            _midiSynth->sendController(ch, e->data1(), e->data2());
        } else {
            _midiOut->sendController(ch, e->data1(), e->data2());
        }
        break;
    }
    case MidiEventType::ProgramChange: {
        int programe = e->data1();
        if (ch == 9 && _lockDrum) {
            programe = _lockDrumNumber;
            _tempEvent = *e;
            _tempEvent.setData1(programe);
            _playingEventPtr = &_tempEvent;
        }
        if (isBassInstrument(e->data1()) && _lockBass) {
            programe = _lockBassBumber;
            _tempEvent = *e;
            _tempEvent.setData1(programe);
            _playingEventPtr = &_tempEvent;
        }

        _midiChannels[ch].setInstrument(programe);
        if (ch != 9)
            _midiChannels[ch].setInstrumentType(MidiHelper::getInstrumentType(programe));

        if (_midiPortNum == -1) {
            _midiSynth->sendProgramChange(ch, programe);
        } else {
            _midiOut->sendProgramChange(ch, programe);
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
//        int n = getNoteNumberToPlay(ch, e->data1());
        if (_midiPortNum == -1) {
            _midiSynth->sendPitchBend(ch, e->data1());
        } else {
            _midiOut->sendPitchBend(ch, e->data1());
        }
        break;
    }
    }

//    qDebug("%d",e->eventType());
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

int MidiPlayer::getNoteNumberToPlay(int ch, int defaultNote)
{
    int n = 0;
    if (ch == 9) {
        if (isSnareNumber(defaultNote) && _lockSnare)
            n = _lockSnareNumber;
        else
            n = defaultNote;
    }
    else {
        n = defaultNote + _midiTranspose;
    }
    return n;
}

int MidiPlayer::getNumberBeatInBar(int numerator, int denominator)
{
    int d = qPow(2, denominator);
    switch (d) {
    case 2:
    case 4:
        return numerator * 1;
    case 8:
        return numerator * 0.5;
    case 16:
        return numerator * 0.25;
    }
}

float MidiPlayer::GetCurrentTempoScale() const
{
    return ((float) tempo_scale) * 0.01;
}

void MidiPlayer::SetCurrentTempoScale (float scale)
{
    tempo_scale = (int) (scale * 100);
}
