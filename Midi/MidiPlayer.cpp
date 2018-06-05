#include "MidiPlayer.h"

#include <QtMath>


MidiPlayer::MidiPlayer(QObject *parent) : QObject(parent)
{
    MidiSequencer *seq1 = new MidiSequencer();
    MidiSequencer *seq2 = new MidiSequencer();
    _midiSeq.push_back(seq1);
    _midiSeq.push_back(seq2);

    _midiSynth  = new MidiSynthesizer();

    if (midiDevices().size() > 0)
    {
        setMidiOut(0);
    }

    connect(seq1, SIGNAL(playingEvent(MidiEvent*)),
            this, SLOT(sendEvent(MidiEvent*)), Qt::DirectConnection);
    connect(seq1, SIGNAL(bpmChanged(int)),
            this, SLOT(onSeqBpmChanged(int)), Qt::DirectConnection);
    connect(seq1, SIGNAL(finished()),
            this, SLOT(onSeqFinished()), Qt::DirectConnection);

    connect(seq2, SIGNAL(playingEvent(MidiEvent*)),
            this, SLOT(sendEvent(MidiEvent*)), Qt::DirectConnection);
    connect(seq2, SIGNAL(bpmChanged(int)),
            this, SLOT(onSeqBpmChanged(int)), Qt::DirectConnection);
    connect(seq2, SIGNAL(finished()),
            this, SLOT(onSeqFinished()), Qt::DirectConnection);
}

MidiPlayer::~MidiPlayer()
{
    for (MidiSequencer *seq : _midiSeq) {
        delete seq;
    }

    for (MidiOut *out : _midiOuts.values()) {
        if (out) {
            out->closePort();
            delete out;
        }
    }

    delete _midiSynth;
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

std::vector<std::string> MidiPlayer::midiInDevices()
{
    std::vector<std::string> inName;
    RtMidiIn in;
    for (int i=0; i<in.getPortCount(); i++) {
        inName.push_back(in.getPortName(i));
    }
    return inName;
}

bool MidiPlayer::isSnareNumber(int num)
{
    if ( (num == 38) || (num == 40) )
        return true;
    else
        return false;
}

bool MidiPlayer::isBassInstrument(int ints)
{
    if ( (ints < 32) || (ints > 39) )
        return false;
    else
        return true;
}

int MidiPlayer::getNumberBeatInBar(int numerator, int denominator)
{
    int value;
    int d = qPow(2, denominator);
    switch (d)
    {
        case 2:
        case 4:
            value = numerator * 1;
            break;
        case 8:
            value = numerator * 0.5;
            break;
        case 16:
            value = numerator * 0.25;
            break;
        default:
            value = 4;
            break;
    }

    if (value == 16 || value == 8)
        value = 4;

    switch (value)
    {
        case 16:
        case 8:
            value = 4;
            break;
        case 12:
            value = 6;
            break;
    }

    return value;
}

QList<SignatureBeat> MidiPlayer::CalculateBeats(MidiFile *midi)
{
    QList<SignatureBeat> beats;
    uint32_t t = midi->events().back()->tick();
    ushort bCount = midi->beatFromTick(t);

    for (MidiEvent *evt : midi->timeSignatureEvents())
    {
        int nBeatInBar = getNumberBeatInBar(evt->data()[0], evt->data()[1]);
        if (nBeatInBar <= 0)
            continue;

        SignatureBeat sb;
        sb.nBeat = midi->beatFromTick(evt->tick());
        sb.nBeatInBar = nBeatInBar;
        beats.append(sb);
    }

    return beats;
}

MidiFile *MidiPlayer::midiFile()
{
    return _midiSeq[_seqIndex]->midiFile();
}

bool MidiPlayer::isPlayerPlaying()
{
    return _midiSeq[_seqIndex]->isSeqPlaying();
}

bool MidiPlayer::isPlayerStopped()
{
    return _midiSeq[_seqIndex]->isSeqStopped();
}

bool MidiPlayer::isPlayerPaused()
{
    return _midiSeq[_seqIndex]->isSeqPaused();
}

bool MidiPlayer::isPlayerFinished()
{
    return _midiSeq[_seqIndex]->isSeqFinished();
}

long MidiPlayer::durationMs()
{
    return _midiSeq[_seqIndex]->durationMs();
}

long MidiPlayer::positionMs()
{
    return _midiSeq[_seqIndex]->positionMs();
}

int MidiPlayer::durationTick()
{
    return _midiSeq[_seqIndex]->durationTick();
}

int MidiPlayer::positionTick()
{
    return _midiSeq[_seqIndex]->positionTick();
}

int MidiPlayer::bpmSpeed()
{
    return _midiSeq[_seqIndex]->bpmSpeed();
}

int MidiPlayer::currentBpm()
{
    return _midiSeq[_seqIndex]->currentBpm();
}

int MidiPlayer::currentBeat()
{
    return _midiSeq[_seqIndex]->currentBeat();
}

int MidiPlayer::beatCount()
{
    return _midiSeq[_seqIndex]->beatCount();
}

bool MidiPlayer::setMidiOut(int portNumber)
{
    if (portNumber != -1 && portNumber >= midiDevices().size())
        return false;

    if (!isPlayerStopped())
        stop(true);

    int oldPort = _midiPortNum;
    bool result = false;

    if (portNumber == -1) {
        _midiSynth->open();
        _midiSynth->setVolume(_volume / 100.0f);
        _midiPortNum = -1;
        result = true;
    } else {
        MidiOut *out = _midiOuts[portNumber];
        if (!out) {
            out = new MidiOut();
            out->openPort(portNumber);
            _midiOuts[portNumber] = out;
        }
        out->setVolume(_volume / 100.0f);
        result = out->isPortOpen();
        _midiPortNum = result ? portNumber : _midiPortNum;
    }

    for (int i=0; i<16; i++) {
        if (_midiChannels[i].port() != oldPort)
            continue;
        _midiChannels[i].setPort(_midiPortNum);
    }

    calculateUsedPort();

    return result;
}

bool MidiPlayer::setMidiIn(int portNumber)
{
    if (portNumber != -1 && portNumber >= midiInDevices().size())
        return false;

    if (portNumber == _midiPortInNum)
        return true;

    _midiPortInNum = portNumber;

    if (portNumber == -1) {
        if (_midiIn != nullptr) {
            _midiIn->closePort();
            delete _midiIn;
            _midiIn = nullptr;
        }
    } else {
        if (_midiIn == nullptr) {
            _midiIn = new RtMidiIn();
            _midiIn->openPort(portNumber);
            _midiIn->setCallback(&midiIncallback, this);
        } else {
            _midiIn->closePort();
            _midiIn->openPort(portNumber);
        }
    }

    return true;
}

bool MidiPlayer::load(const QString &file, bool seekFileChunkID)
{
    if (!isPlayerStopped())
        stop(true);

    if (!_midiSeq[_seqIndex]->load(file, seekFileChunkID))
        return false;

    _midiTranspose = 0;

    for (int i=0; i<16; i++) {
        _midiChannels[i].setInstrument(0);
        _midiChannels[i].setVolume(100);
        _midiChannels[i].setPan(64);
        _midiChannels[i].setReverb(0);
        _midiChannels[i].setChorus(0);
        _midiChannels[i].setInstrumentType(InstrumentType::Piano);
    }
    _midiChannels[9].setInstrumentType(InstrumentType::PercussionEtc);

    _midiSynth->compactSoundfont();

    emit loaded();

    return true;
}

void MidiPlayer::play()
{
    if (isPlayerStopped()) {
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
        emit sendedEvent(&ev);
    }
    _midiSeq[_seqIndex]->start();
}

void MidiPlayer::stop(bool resetPos)
{
    if (isPlayerStopped())
        return;

    _midiSeq[_seqIndex]->stop(resetPos);

    sendAllNotesOff();
}

void MidiPlayer::setVolume(int v)
{
    if (v < 0) _volume = 0;
    else if (v > 100) _volume = 100;
    else _volume = v;

    _midiSynth->setVolume(_volume / 100.0f);
    for (MidiOut *out : _midiOuts.values()) {
        if (!out) continue;
        out->setVolume(_volume / 100.0f);
    }
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

    _midiSynth->sendProgramChange(ch, v);
    for (MidiOut *out : _midiOuts.values()) {
        if (!out) continue;
        out->sendProgramChange(ch, v);
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
        if (_midiChannels[i].isSolo()) {
            us = true;
        } else {
            sendAllNotesOff(i);
        }
    }

    _useSolo = us;

    if (isPlayerPlaying()) {
        for (int i=0; i<16; i++) {
            if (_midiChannels[i].isSolo()) {
                continue;
            }
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
    _midiSeq[_seqIndex]->setPositionTick(t);
}

void MidiPlayer::setTranspose(int t)
{
    if (_midiTranspose == -12 || _midiTranspose == 12)
        return;

    _midiTranspose = t;

    if (isPlayerPlaying()) {
        for (int i=0; i<16; i++) {
            if (i == 9)
                continue;
            sendAllNotesOff(i);
        }
    }
}

void MidiPlayer::setBpmSpeed(int sp)
{
    _midiSeq[_seqIndex]->setBpmSpeed(sp);
}

void MidiPlayer::setLockDrum(bool lock, int number)
{
    _lockDrum = lock;
    _lockDrumNumber = number;

    if (lock && !isPlayerStopped()) {
        MidiEvent ev;
        ev.setEventType(MidiEventType::ProgramChange);
        ev.setChannel(9);
        ev.setData1(number);
        sendEvent(&ev);
        emit sendedEvent(&ev);
    }
}

void MidiPlayer::setLockSnare(bool lock, int number)
{
    if ( !isSnareNumber(number) )
        return;

    _lockSnare = lock;
    _lockSnareNumber = number;

    if (isPlayerPlaying()) {
        sendAllNotesOff(9);
    }
}

void MidiPlayer::setLockBass(bool lock, int number)
{
    if ( !isBassInstrument(number) )
        return;

    _lockBass = lock;
    _lockBassBumber = number;

    if (isPlayerStopped())
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
            emit sendedEvent(&ev);
        }
    }
}

void MidiPlayer::setMapChannelOutput(int ch, int port)
{
    if (port != -1 && port >= midiDevices().size())
        return;

    if (port == _midiChannels[ch].port())
        return;

    if (port == -1)
    {
        if (!_midiSynth->isOpened()) {
            _midiSynth->open();
            _midiSynth->setVolume(_volume / 100.0f);
        }

        if (!this->isPlayerStopped()) {
            _midiSynth->sendProgramChange(ch, _midiChannels[ch].instrument());
            _midiSynth->sendController(ch, 7, _midiChannels[ch].volume());
            _midiSynth->sendController(ch, 10, _midiChannels[ch].pan());
            _midiSynth->sendController(ch, 91, _midiChannels[ch].reverb());
            _midiSynth->sendController(ch, 93, _midiChannels[ch].chorus());
        }
    }
    else
    {
        MidiOut *out = _midiOuts[port];
        if (!out) {
            out = new MidiOut();
            out->openPort(port);
            out->setVolume(_volume / 100.0f);
            _midiOuts[port] = out;
        }

        if (!this->isPlayerStopped()) {
            out->sendProgramChange(ch, _midiChannels[ch].instrument());
            out->sendController(ch, 7, _midiChannels[ch].volume());
            out->sendController(ch, 10, _midiChannels[ch].pan());
            out->sendController(ch, 91, _midiChannels[ch].reverb());
            out->sendController(ch, 93, _midiChannels[ch].chorus());
        }
    }

    if (isPlayerPlaying()) {
        _midiChannels[ch].setChangingPort(true);
        sendAllNotesOff(ch);
        _midiChannels[ch].setPort(port);
        _midiChannels[ch].setChangingPort(false);
    } else {
        _midiChannels[ch].setPort(port);
    }

    this->calculateUsedPort();
}

void MidiPlayer::receiveMidiIn(std::vector<unsigned char> *message)
{
    _midiInEvent.setMessage(message);
    this->sendEvent(&_midiInEvent);
}

void MidiPlayer::sendEvent(MidiEvent *e)
{
    _playingEventPtr = e;

    if (e->eventType() == MidiEventType::Controller
        || e->eventType() == MidiEventType::ProgramChange) {
        sendEventToDevices(e);
    } else {
        if (_midiChannels[e->channel()].isMute() == false) {
            if (_useSolo) {
                if (_midiChannels[e->channel()].isSolo()) {
                    sendEventToDevices(e);
                }
            } else {
                sendEventToDevices(e);
            }
        }
    }

    emit sendedEvent(_playingEventPtr);
}

void MidiPlayer::onSeqFinished()
{
    emit finished();
}

void MidiPlayer::onSeqBpmChanged(int bpm)
{
    emit bpmChanged(bpm);
}

void MidiPlayer::sendEventToDevices(MidiEvent *e)
{
    int ch = e->channel();

    switch (e->eventType()) {
        case MidiEventType::NoteOff: {
            int n = getNoteNumberToPlay(ch, e->data1());
            if (_midiChannels[ch].port() == -1) {
                _midiSynth->sendNoteOff(ch, n, e->data2());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendNoteOff(ch, n, e->data2());
            }
            break;
        }
        case MidiEventType::NoteOn: {
            if (_midiChannels[ch].isChangingPort())
                break;
            int n = getNoteNumberToPlay(ch, e->data1());
            if (_midiChannels[ch].port() == -1) {
                _midiSynth->sendNoteOn(ch, n, e->data2());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendNoteOn(ch, n, e->data2());
            }
            break;
        }
        case MidiEventType::NoteAftertouch: {
            int n = getNoteNumberToPlay(ch, e->data1());
            if (_midiChannels[ch].port() == -1) {
                _midiSynth->sendNoteAftertouch(ch, n, e->data2());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendNoteAftertouch(ch, n, e->data2());
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

            if (_midiChannels[ch].port() == -1) {
                _midiSynth->sendController(ch, e->data1(), e->data2());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendController(ch, e->data1(), e->data2());
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
            if (ch != 9) {
                _midiChannels[ch].setInstrumentType(MidiHelper::getInstrumentType(programe));
            }

            if (_midiChannels[ch].port() == -1) {
                _midiSynth->sendProgramChange(ch, programe);
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendProgramChange(ch, programe);
            }
            break;
        }
        case MidiEventType::ChannelAftertouch: {
            if (_midiChannels[ch].port() == -1) {
                _midiSynth->sendChannelAftertouch(ch, e->data1());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendChannelAftertouch(ch, e->data1());
            }
            break;
        }
        case MidiEventType::PitchBend: {
            if (_midiChannels[ch].port() == -1) {
                _midiSynth->sendPitchBend(ch, e->data1());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendPitchBend(ch, e->data1());
            }
            break;
        }
        default:
            break;
    }
}

void MidiPlayer::sendAllNotesOff(int ch)
{
    if (_midiChannels[ch].port() == -1) {
        _midiSynth->sendAllNotesOff(ch);
    } else {
        _midiOuts[_midiChannels[ch].port()]->sendAllNotesOff(ch);
    }
}

void MidiPlayer::sendAllNotesOff()
{
    for (int i=0; i<16; i++) {
        this->sendAllNotesOff(i);
    }
}

void MidiPlayer::sendResetAllControllers(int ch)
{
    if (_midiChannels[ch].port() == -1) {
            _midiSynth->sendResetAllControllers(ch);
    } else {
        _midiOuts[_midiChannels[ch].port()]->sendResetAllControllers(ch);
    }
}

void MidiPlayer::sendResetAllControllers()
{
    for (int i=0; i<16; i++) {
        sendResetAllControllers(i);
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

void MidiPlayer::calculateUsedPort()
{
    // calculate used port
    // list port all channel
    bool usedSynth = false;
    QList<int> usedPort;
    for (int i=0; i<16; i++) {
        int p = _midiChannels[i].port();
        usedPort.append(p);
        if (p == -1)
            usedSynth = true;
    }

    // remove port not used
    if (!usedSynth) {
        _midiSynth->close();
    }
    for (int pNumber : _midiOuts.keys()) {

        if (usedPort.indexOf(pNumber) != -1)
            continue;

        _midiOuts[pNumber]->closePort();;
        delete _midiOuts.take(pNumber);
    }
}

void midiIncallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
    if (message->size() < 2)
        return;

    if ((message->at(0) & 0xF0) != 0xF0) {

        ((MidiPlayer*)(userData))->receiveMidiIn(message);

    }
}
