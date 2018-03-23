#include "MidiPlayer.h"

#include <QtMath>


MidiPlayer::MidiPlayer(QObject *parent) : QObject(parent)
{
    _midiSeq.push_back(new MidiSequencer());
    _midiSeq.push_back(new MidiSequencer());

    _midiSynth  = new MidiSynthesizer();
}

MidiPlayer::~MidiPlayer()
{
    for (MidiSequencer *seq : _midiSeq) {
        delete seq;
    }

    for (MidiOut *out : _midiOuts.values()) {
        if (out) {
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
    int d = qPow(2, denominator);
    switch (d) {
        case 2:
        case 4:
            return numerator * 1;
        case 8:
            return numerator * 0.5;
        case 16:
            return numerator * 0.25;
        default:
            return 4;
    }
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

void MidiPlayer::sendEventToDevices(MidiEvent *e)
{
    int ch = e->channel();

    switch (e->eventType()) {
        case MidiEventType::NoteOff: {
            int n = getNoteNumberToPlay(ch, e->data1());
            if (_midiPortNum == -1) {
                _midiSynth->sendNoteOff(ch, n, e->data2());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendNoteOff(ch, n, e->data2());
            }
            break;
        }
        case MidiEventType::NoteOn: {
            int n = getNoteNumberToPlay(ch, e->data1());
            if (_midiPortNum == -1) {
                _midiSynth->sendNoteOn(ch, n, e->data2());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendNoteOn(ch, n, e->data2());
            }
            break;
        }
        case MidiEventType::NoteAftertouch: {
            int n = getNoteNumberToPlay(ch, e->data1());
            if (_midiPortNum == -1) {
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

            if (_midiPortNum == -1) {
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
            if (ch != 9)
                _midiChannels[ch].setInstrumentType(MidiHelper::getInstrumentType(programe));

            if (_midiPortNum == -1) {
                _midiSynth->sendProgramChange(ch, programe);
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendProgramChange(ch, programe);
            }
            break;
        }
        case MidiEventType::ChannelAftertouch: {
            if (_midiPortNum == -1) {
                _midiSynth->sendChannelAftertouch(ch, e->data1());
            } else {
                _midiOuts[_midiChannels[ch].port()]->sendChannelAftertouch(ch, e->data1());
            }
            break;
        }
        case MidiEventType::PitchBend: {
            if (_midiPortNum == -1) {
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
