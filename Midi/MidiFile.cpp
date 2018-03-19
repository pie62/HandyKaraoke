#include "MidiFile.h"

#include <iostream>
#include <iomanip>

#include <cstdlib>

// ========================================================
bool isGreaterThan(MidiEvent* e1, MidiEvent* e2)
{
    return (e1->tick() < e2->tick());
}
quint16 readUInt16(QFile *in) {
    unsigned char buffer[2];
    in->read((char*)&buffer, 2);
    return ((uint16_t)(buffer[0]) << 8) | (uint16_t)(buffer[1]);
}

quint32 readUInt32(QFile *in) {
    unsigned char buffer[4];
    in->read((char*)&buffer, 4);
    return ((uint32_t)(buffer[0]) << 24) | ((uint32_t)(buffer[1]) << 16) |
           ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
}

quint32 readVariableLengthQuantity(QFile *in) {
    unsigned char b;
    uint32_t value = 0;
    do {
        in->getChar((char*)&b);
        value = (value << 7) | (b & 0x7F);
    } while ((b & 0x80) == 0x80 && !in->atEnd());

    return value;
}
// ========================================================

MidiFile::MidiFile()
{
    clear();
}

MidiFile::~MidiFile()
{
    clear();
}

QList<MidiEvent *> MidiFile::controllerAndProgramEvents()
{
    QList<MidiEvent*> evnts = fControllerEvents;
    for (MidiEvent *e : fProgramChangeEvents)
        evnts.append(e);

    qStableSort(evnts.begin(), evnts.end(), isGreaterThan);
    return evnts;
}

void MidiFile::clear()
{
    fFormatType = 1;
    fNumOfTracks = 0;
    fResolution = 0;
    fDivision = PPQ;
    for (MidiEvent *e : fEvents)
        delete e;
    fEvents.clear();
    fTempoEvents.clear();
    fControllerEvents.clear();
    fProgramChangeEvents.clear();
    fTimeSignatureEvents.clear();
}

bool MidiFile::read(const QString &file, bool seekFileChunkID)
{
    if (!QFile::exists(file))
        return false;

    QFile in(file);

    return read(&in, seekFileChunkID);
}

bool MidiFile::read(QFile *in, bool seekFileChunkID)
{
    if (!in->exists() || !in->open(QFile::ReadOnly))
        return false;

    clear();

    char chunkID[4];
    qint64 chunkSize = 0, chunkStart = 0;

    in->read((char*)chunkID, 4);

    if (seekFileChunkID == false) {
        if (memcmp(chunkID, "MThd", 4) != 0){
            in->close();
            return false;
        }
    }

    chunkSize = readUInt32(in);
    chunkStart = in->pos();

    if (chunkSize != 6) {
        in->close();
        return false;
    }

    fFormatType = readUInt16(in);
    fNumOfTracks = readUInt16(in);

    unsigned char divResolution[2];
    in->read((char*)divResolution, 2);

    switch ((signed char)(divResolution[0])) {
    case SMPTE24:
        fDivision = SMPTE24;
        fResolution = divResolution[1];
        break;
    case SMPTE25:
        fDivision = SMPTE25;
        fResolution = divResolution[1];
        break;
    case SMPTE30DROP:
        fDivision = SMPTE30DROP;
        fResolution = divResolution[1];
        break;
    case SMPTE30:
        fDivision = SMPTE30;
        fResolution = divResolution[1];
        break;
    default:
        fDivision = PPQ;
        fResolution = divResolution[1] | divResolution[0] << 8;
        break;
    }

    for (int t=0; t<fNumOfTracks; t++) {

        in->read((char*)chunkID, 4);
        chunkSize = readUInt32(in);
        chunkStart = in->pos();

        if (memcmp(chunkID, "MTrk", 4) != 0) {
            clear();
            in->close();
            return false;
        }

        uint32_t tick = 0, delta = 0;
        unsigned char status, runningStatus = 0;
        //bool endOfTrack = false;

        while (in->pos() < (chunkStart + chunkSize) && !in->atEnd()) {

            delta = readVariableLengthQuantity(in);
            tick += delta;
            in->getChar((char*)&status);

            if ((status & 0x80) == 0) {
                status = runningStatus;
                in->seek(in->pos() - 1);
            } else {
                runningStatus = status;
            }

            switch (status & 0xF0) {
            case 0x80: {
                int ch = status & 0x0F;
                char d1, d2;
                in->getChar(&d1);
                in->getChar(&d2);
                createMidiEvent(t, tick, delta, MidiEventType::NoteOff, ch, d1, d2);
                break;
            }
            case 0x90: {
                int ch = status & 0x0F;
                char d1, d2;
                in->getChar(&d1);
                in->getChar(&d2);
                if (d2 != 0) {
                    createMidiEvent(t, tick, delta, MidiEventType::NoteOn, ch, d1, d2);
                } else {
                    createMidiEvent(t, tick, delta, MidiEventType::NoteOff, ch, d1, 0);
                }
                break;
            }
            case 0xA0: {
                int ch = status & 0x0F;
                char d1, d2;
                in->getChar(&d1);
                in->getChar(&d2);
                createMidiEvent(t, tick, delta, MidiEventType::NoteAftertouch, ch, d1, d2);
                break;
            }
            case 0xB0: {
                int ch = status & 0x0F;
                char d1, d2;
                in->getChar(&d1);
                in->getChar(&d2);
                MidiEvent *e = createMidiEvent(t, tick, delta, MidiEventType::Controller, ch, d1, d2);
                fControllerEvents.append(e);
                break;
            }
            case 0xC0: {
                int ch = status & 0x0F;
                char d1;
                in->getChar(&d1);
                MidiEvent *e = createMidiEvent(t, tick, delta, MidiEventType::ProgramChange, ch, d1, 0);
                fProgramChangeEvents.append(e);
                break;
            }
            case 0xD0: {
                int ch = status & 0x0F;
                char d1;
                in->getChar(&d1);
                createMidiEvent(t, tick, delta, MidiEventType::ChannelAftertouch, ch, d1, 0);
                break;
            }
            case 0xE0: {
                int ch = status & 0x0F;
                char d1, d2;
                in->getChar(&d1);
                in->getChar(&d2);
                int pitch = ((d2 & 0x7F) << 7) | (d1 & 0x7F);
                createMidiEvent(t, tick, delta, MidiEventType::PitchBend, ch, pitch, 0);
                break;
            }
            case 0xF0:
                QByteArray data;
                int lenght = 0;
                switch (status) {
                    case 0xF0:
                    case 0xF7:
                        lenght = readVariableLengthQuantity(in) + 1;
                        data[0] = status;
                        data += in->read(lenght - 1);
                        createSysExEvent(t, tick, delta, data);
                        break;
                    case 0xFF:
                        char number;
                        in->getChar(&number);
                        lenght = readVariableLengthQuantity(in);
                        data = in->read(lenght);
                        if (number == 0x2F && in->pos() < (chunkStart + chunkSize)
                                && !in->atEnd()) {
                            in->read(1);
                        }
                        createMetaEvent(t, tick, delta, number, data);
                        break;
                }
                break;
            } // End Switch

        } // End while

    } // For loop read tracks

    qStableSort(fEvents.begin(), fEvents.end(), isGreaterThan);
    qStableSort(fTempoEvents.begin(), fTempoEvents.end(), isGreaterThan);
    qStableSort(fControllerEvents.begin(), fControllerEvents.end(), isGreaterThan);
    qStableSort(fProgramChangeEvents.begin(), fProgramChangeEvents.end(), isGreaterThan);
    qStableSort(fTimeSignatureEvents.begin(), fTimeSignatureEvents.end(), isGreaterThan);

    in->close();

    return true;
}

MidiEvent *MidiFile::createMidiEvent(int track, uint32_t tick, uint32_t delta, MidiEventType evType, int ch, int data1, int data2)
{
    MidiEvent *e = new MidiEvent();
    e->setTrack(track);
    e->setTick(tick);
    e->setDelta(delta);
    e->setEventType(evType);
    e->setChannel(ch);
    e->setData1(data1);
    e->setData2(data2);
    fEvents.append(e);

    return e;
}

MidiEvent *MidiFile::createMetaEvent(int track, uint32_t tick, uint32_t delta, int number, QByteArray data)
{
    MidiEvent *me = new MidiEvent();
    me->setTrack(track);
    me->setTick(tick);
    me->setDelta(delta);
    me->setEventType(MidiEventType::Meta);
    me->setMetaType(number);
    me->setData(data);
    fEvents.append(me);
    if (me->metaEventType() == MidiMetaType::SetTempo)
        fTempoEvents.append(me);
    if (me->metaEventType() == MidiMetaType::TimeSignature)
        fTimeSignatureEvents.append(me);

    return me;
}

MidiEvent *MidiFile::createSysExEvent(int track, uint32_t tick, uint32_t delta, QByteArray data)
{
    MidiEvent *e = new MidiEvent();
    e->setTrack(track);
    e->setTick(tick);
    e->setDelta(delta);
    e->setEventType(MidiEventType::SysEx);
    e->setData(data);
    fEvents.append(e);

    return e;
}

float MidiFile::beatFromTick(uint32_t tick)
{
    switch (fDivision) {
    case PPQ:
        return (float)(tick) / fResolution;
    case SMPTE24:
        return (float)(tick) / 24.0;
    case SMPTE25:
        return (float)(tick) / 25.0;
    case SMPTE30DROP:
        return (float)(tick) / 29.97;
    case SMPTE30:
        return (float)(tick) / 30.0;
    default:
        return 0.0f;
    }
}

float MidiFile::timeFromTick(uint32_t tick, int bpmSpeed)
{
    switch (fDivision) {
    case PPQ: {
        float tempo_event_time = 0.0;
        uint32_t tempo_event_tick = 0;
        float tempo = 120.0 + bpmSpeed;

        for (MidiEvent* e : fTempoEvents) {
            if (e->tick() >= tick) {
                break;
            }
            tempo_event_time +=
                (((float)(e->tick() - tempo_event_tick)) / fResolution / (tempo / 60));
            tempo_event_tick = e->tick();
            tempo = e->bpm() + bpmSpeed;
        }

        float time =
            tempo_event_time + (((float)(tick - tempo_event_tick)) / fResolution / (tempo / 60));
        return time;
    }
    case SMPTE24:
        return (float)(tick) / (fResolution * 24.0);
    case SMPTE25:
        return (float)(tick) / (fResolution * 25.0);
    case SMPTE30DROP:
        return (float)(tick) / (fResolution * 29.97);
    case SMPTE30:
        return (float)(tick) / (fResolution * 30.0);
    default:
        return 0.0f;
    }
}

uint32_t MidiFile::tickFromTime(float time, int bpmSpeed)
{
    switch (fDivision) {
    case PPQ: {
        float tempo_event_time = 0.0;
        uint32_t tempo_event_tick = 0;
        float tempo = 120.0 + bpmSpeed;

        for (MidiEvent* e : fTempoEvents) {
            float next_tempo_event_time =
                tempo_event_time +
                (((float)(e->tick() - tempo_event_tick)) / fResolution / (tempo / 60));
            if (next_tempo_event_time >= time) break;
            tempo_event_time = next_tempo_event_time;
            tempo_event_tick = e->tick();
            tempo = e->bpm() + bpmSpeed;
        }

        return tempo_event_tick + (uint32_t)((time - tempo_event_time) * (tempo / 60) * fResolution);
    }
    case SMPTE24:
        return (uint32_t)(time * fResolution * 24.0);
    case SMPTE25:
        return (uint32_t)(time * fResolution * 25.0);
    case SMPTE30DROP:
        return (uint32_t)(time * fResolution * 29.97);
    case SMPTE30:
        return (uint32_t)(time * fResolution * 30.0);
    default:
        return 0;
    }
}

uint32_t MidiFile::tickFromTimeMs(long msTime, int bpmSpeed)
{
    switch (fDivision) {
    case PPQ: {
        float tempo_event_time = 0.0;
        uint32_t tempo_event_tick = 0;
        float tempo = 120.0 + bpmSpeed;

        for (MidiEvent* e : fTempoEvents) {
            float next_tempo_event_time =
                tempo_event_time +
                (((float)(e->tick() - tempo_event_tick)) / fResolution / (tempo / 60000));
            if (next_tempo_event_time >= msTime) break;
            tempo_event_time = next_tempo_event_time;
            tempo_event_tick = e->tick();
            tempo = e->bpm() + bpmSpeed;
        }

        return tempo_event_tick + (uint32_t)((msTime - tempo_event_time) * (tempo / 60000) * fResolution);
    }
    case SMPTE24:
        return (uint32_t)(msTime * fResolution * 24.0) * 1000;
    case SMPTE25:
        return (uint32_t)(msTime * fResolution * 25.0) * 1000;
    case SMPTE30DROP:
        return (uint32_t)(msTime * fResolution * 29.97) * 1000;
    case SMPTE30:
        return (uint32_t)(msTime * fResolution * 30.0) * 1000;
    default:
        return 0;
    }
}

void MidiFile::printEvents()
{
    using namespace std;

    cout << fResolution << endl;

    /*
    { // Add
        long mark = fResolution * 4;
        long lmark = mark * 2;
        QList<MidiEvent*> nEv, lEv, evs, tev;

        for (MidiEvent *e : events()) {

            if (e->tick() >= lmark) {
                break;
            }

            if (e->eventType() == MidiEventType::NoteOn
                    || e->eventType() == MidiEventType::NoteOff ) {

                if (e->tick() >= mark) {
                    lEv.append(e);
                }
                else {
                    nEv.append(e);
                }
            }

        }

        long m = 0;
        for (int i=0; i<4; i++) {

            for (MidiEvent *e : nEv) {
                MidiEvent *ev = new MidiEvent();
                *ev = *e;
                ev->setTick(ev->tick() + m);
                evs.append(ev);
            }

            m = fResolution * 4 + m;

        }

        for (MidiEvent *e : fTempoEvents) {
            MidiEvent *ev = new MidiEvent();
            *ev = *e;
            evs.append(ev);
            tev.append(ev);
        }

        for (MidiEvent *e : fEvents) {
            delete e;
        }

        qStableSort(evs.begin(), evs.end(), isGreaterThan);
        qStableSort(tev.begin(), tev.end(), isGreaterThan);

        fEvents.clear();
        fTempoEvents.clear();

        fEvents = evs;
        fTempoEvents = tev;
    }
    */

    { // Print
        long mark = fResolution * 4;
        cout << "Mark " << mark << endl;

        for (MidiEvent *e : events()) {

            if (e->tick() >= mark ) {
                mark = fResolution * 4 + mark;
                cout << "====================================================" << endl;
            }

            if (e->eventType() == MidiEventType::NoteOn )
                cout << e->tick() << setw(6) << e->channel() << setw(6) << e->data1() << endl;

        }
    }

}

int MidiFile::firstBpm(const QString &file)
{
    if (!QFile::exists(file))
        return 0;

    QFile in(file);

    int bpm = firstBpm(&in);
    return bpm;
}

int MidiFile::firstBpm(QFile *in)
{
    if (!in->exists() || !in->open(QFile::ReadOnly))
        return false;

    uchar cId[4];
    in->read((char*)cId, 4);

    int cSize = readUInt32(in);
    if (cSize != 6) {
        in->close();
        return 0;
    }

    int fType = readUInt16(in);
    int nTracks = readUInt16(in);

    if (fType == 2) {
        in->close();
        return 0;
    }

    unsigned char divResolution[2];
    in->read((char*)divResolution, 2);

    for (int i=0; i<nTracks; i++) {

        in->read((char*)cId, 4);
        cSize = readUInt32(in);

        if (memcmp(cId, "MTrk", 4) != 0) {
            in->close();
            return 0;
        }

        in->seek(in->pos() + cSize);
    }

    // End check file

    in->seek(0);

    QByteArray buffer = in->readAll();
    int index = buffer.indexOf(0xFF51, 0);

    if (index == -1) {
        in->close();
        return 120;
    }
    else {
        in->seek(index);
        char metaNumber;//, lenght;
        in->getChar(&metaNumber);
        //in->getChar(&lenght);
        int lenght = readVariableLengthQuantity(in);

        // wrong
        if (metaNumber != 0x51 || lenght != 3) {
            in->close();
            return 120;
        }

        unsigned char data[3];
        in->read((char*)data, 3);
        int32_t midi_tempo = (data[0] << 16) | (data[1] << 8) | data[2];

        in->close();

        return 60000000 / midi_tempo;
    }
}


