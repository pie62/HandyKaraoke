#include "MidiFile2.h"

#include <cstdlib>

// ========================================================
bool smf_isGreaterThan(MidiEvent* e1, MidiEvent* e2)
{
    return (e1->tick() < e2->tick());
}
quint16 smf_readUInt16(QFile *in) {
    unsigned char buffer[2];
    in->read((char*)&buffer, 2);
    return ((uint16_t)(buffer[0]) << 8) | (uint16_t)(buffer[1]);
}

quint32 smf_readUInt32(QFile *in) {
    unsigned char buffer[4];
    in->read((char*)&buffer, 4);
    return ((uint32_t)(buffer[0]) << 24) | ((uint32_t)(buffer[1]) << 16) |
           ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
}

quint32 smf_readVariableLengthQuantity(QFile *in) {
    unsigned char b;
    uint32_t value = 0;
    do {
        in->getChar((char*)&b);
        value = (value << 7) | (b & 0x7F);
    } while ((b & 0x80) == 0x80 && !in->atEnd());

    return value;
}
// ========================================================

MidiFile2::MidiFile2()
{
    clear();
}

MidiFile2::~MidiFile2()
{
    clear();
}

QList<MidiEvent *> MidiFile2::controllerAndProgramEvents()
{
    QList<MidiEvent*> evnts = fControllerEvents;
    for (MidiEvent *e : fProgramChangeEvents)
        evnts.append(e);

    qStableSort(evnts.begin(), evnts.end(), smf_isGreaterThan);
    return evnts;
}

void MidiFile2::clear()
{
    fFormatType = 1;
    fNumOfTracks = 0;
    fResolution = 0;
    fDivision = PPQ;

    fTempoEvents.clear();
    fControllerEvents.clear();
    fProgramChangeEvents.clear();
    fTimeSignatureEvents.clear();

    for (QList<MidiEvent*> evts : fTracksEvents) {
        for (MidiEvent *ev : evts)
            delete ev;
    }

    fTracksEvents.clear();
}

bool MidiFile2::read(const QString &file, bool seekFileChunkID)
{
    if (!QFile::exists(file))
        return false;

    QFile in(file);

    return read(&in, seekFileChunkID);
}

bool MidiFile2::read(QFile *in, bool seekFileChunkID)
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

    chunkSize = smf_readUInt32(in);
    chunkStart = in->pos();

    if (chunkSize != 6) {
        in->close();
        return false;
    }

    fFormatType = smf_readUInt16(in);
    fNumOfTracks = smf_readUInt16(in);

    for (int i=0; i<fNumOfTracks; i++)
        fTracksEvents.append(QList<MidiEvent*>());

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
        chunkSize = smf_readUInt32(in);
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

            delta = smf_readVariableLengthQuantity(in);
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
                        lenght = smf_readVariableLengthQuantity(in) + 1;
                        data[0] = status;
                        data += in->read(lenght - 1);
                        createSysExEvent(t, tick, delta, data);
                        break;
                    case 0xFF:
                        char number;
                        in->getChar(&number);
                        lenght = smf_readVariableLengthQuantity(in);
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

        } // End while read events in track

    } // For loop read tracks

    qStableSort(fTempoEvents.begin(), fTempoEvents.end(), smf_isGreaterThan);
    qStableSort(fControllerEvents.begin(), fControllerEvents.end(), smf_isGreaterThan);
    qStableSort(fProgramChangeEvents.begin(), fProgramChangeEvents.end(), smf_isGreaterThan);
    qStableSort(fTimeSignatureEvents.begin(), fTimeSignatureEvents.end(), smf_isGreaterThan);

    in->close();

    return true;
}

MidiEvent *MidiFile2::createMidiEvent(int track, uint32_t tick, uint32_t delta, MidiEventType evType, int ch, int data1, int data2)
{
    MidiEvent *e = new MidiEvent();
    e->setTrack(track);
    e->setTick(tick);
    e->setDelta(delta);
    e->setEventType(evType);
    e->setChannel(ch);
    e->setData1(data1);
    e->setData2(data2);

    fTracksEvents[track].append(e);

    return e;
}

MidiEvent *MidiFile2::createMetaEvent(int track, uint32_t tick, uint32_t delta, int number, QByteArray data)
{
    MidiEvent *me = new MidiEvent();
    me->setTrack(track);
    me->setTick(tick);
    me->setDelta(delta);
    me->setEventType(MidiEventType::Meta);
    me->setMetaType(number);
    me->setData(data);

    fTracksEvents[track].append(me);

    if (me->metaEventType() == MidiMetaType::SetTempo)
        fTempoEvents.append(me);
    if (me->metaEventType() == MidiMetaType::TimeSignature)
        fTimeSignatureEvents.append(me);

    return me;
}

MidiEvent *MidiFile2::createSysExEvent(int track, uint32_t tick, uint32_t delta, QByteArray data)
{
    MidiEvent *e = new MidiEvent();
    e->setTrack(track);
    e->setTick(tick);
    e->setDelta(delta);
    e->setEventType(MidiEventType::SysEx);
    e->setData(data);

    fTracksEvents[track].append(e);

    return e;
}

float MidiFile2::beatFromTick(uint32_t tick)
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

float MidiFile2::timeFromTick(uint32_t tick, int bpmSpeed)
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

uint32_t MidiFile2::tickFromTime(float time, int bpmSpeed)
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

uint32_t MidiFile2::tickFromTimeMs(long msTime, int bpmSpeed)
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

int MidiFile2::firstBpm(const QString &file)
{
    if (!QFile::exists(file))
        return 0;

    QFile in(file);

    int bpm = firstBpm(&in);
    return bpm;
}

int MidiFile2::firstBpm(QFile *in)
{
    if (!in->exists() || !in->open(QFile::ReadOnly))
        return false;

    uchar cId[4];
    in->read((char*)cId, 4);

    int cSize = smf_readUInt32(in);
    if (cSize != 6) {
        in->close();
        return 0;
    }

    int fType = smf_readUInt16(in);
    int nTracks = smf_readUInt16(in);

    if (fType == 2) {
        in->close();
        return 0;
    }

    unsigned char divResolution[2];
    in->read((char*)divResolution, 2);

    for (int i=0; i<nTracks; i++) {

        in->read((char*)cId, 4);
        cSize = smf_readUInt32(in);

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
        int lenght = smf_readVariableLengthQuantity(in);

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
