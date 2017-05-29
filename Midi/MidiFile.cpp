//
// Created by noob on 16/5/2560.
//

#include "MidiFile.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

MidiFile::MidiFile() {
    clear();
}

MidiFile::~MidiFile() {
    clear();
}

int MidiFile::bpm()
{
    int bpm = 120;
    if (fTempoEvent.size() > 0) {
        bpm = fTempoEvent[0]->tempoBPM();
    }

    return bpm;
}

void MidiFile::clear() {
    fFormatType = 1;
    fNumOfTracks = 0;
    fResorution = 0;
    fDivision = PPQ;
    for (MidiEvent *e : fEvents)
        delete e;
    fEvents.clear();
    fTempoEvent.clear();
}

bool fileExists(const std::string& filename) {
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}

uint16_t readUInt16(std::ifstream *in) {
    unsigned char buffer[2];
    in->read((char*)buffer, 2);
    return ((uint16_t)(buffer[0]) << 8) | (uint16_t)(buffer[1]);
}

uint32_t readUInt32(std::ifstream *in) {
    unsigned char buffer[4];
    in->read((char*)&buffer, 4);
    return ((uint32_t)(buffer[0]) << 24) | ((uint32_t)(buffer[1]) << 16) |
           ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
}

uint32_t readVariableLengthQuantity(std::ifstream *in) {
    unsigned char b;
    uint32_t value = 0;
    do {
        b = in->get();
        value = (value << 7) | (b & 0x7F);
    } while ((b & 0x80) == 0x80 && !in->eof());

    return value;
}

bool eventTickCompare(MidiEvent *e1, MidiEvent *e2) {
    return e1->tick() < e2->tick();
}

bool MidiFile::read(std::string filename) {

    if (!fileExists(filename))
        return false;

    std::ifstream in(filename, std::ios::in|std::ios::binary);
    if (!in.is_open())
        return false;

    clear();

    char chunkID[4];
    uint32_t chunkSize = 0, chunkStart = 0;

    in.read((char*)chunkID, 4);

    if (memcmp(chunkID, "MThd", 4) != 0) {
        if (memcmp(chunkID, "Lock", 4) != 0) {
            std::cout << "File chunk ID is invalid." << std::endl;
            return false;
        }
    }

    chunkSize = readUInt32(&in);
    chunkStart = in.tellg();

    if (chunkSize != 6) {
        std::cout << "Chunk size is invalid." << std::endl;
        return false;
    }

    fFormatType = readUInt16(&in);
    fNumOfTracks = readUInt16(&in);

    unsigned char divResolution[2];
    in.read((char*)divResolution, 2);

    switch ((signed char)(divResolution[0])) {
        case SMPTE24:
            fDivision = SMPTE24;
            fResorution = divResolution[1];
            break;
        case SMPTE25:
            fDivision = SMPTE25;
            fResorution = divResolution[1];
            break;
        case SMPTE30DROP:
            fDivision = SMPTE30DROP;
            fResorution = divResolution[1];
            break;
        case SMPTE30:
            fDivision = SMPTE30;
            fResorution = divResolution[1];
            break;
        default:
            fDivision = PPQ;
            fResorution = divResolution[1] | divResolution[0] << 8;
            break;
    }

    for (int t=0; t<fNumOfTracks; t++) {

        in.read((char*)chunkID, 4);
        chunkSize = readUInt32(&in);
        chunkStart = in.tellg();

        if (memcmp(chunkID, "MTrk", 4) != 0) {
            clear();
            std::cout << "Track " << t << " chunk ID is invalid." << std::endl;
            return false;
        }

        uint32_t tick = 0, delta = 0;
        unsigned char status, runningStatus = 0;
        bool endOfTrack = false;

        while ((in.tellg() < (chunkStart + chunkSize)) && !in.eof()) {
            delta = readVariableLengthQuantity(&in);
            tick += delta;
            status = in.get();

            if ((status & 0x80) == 0) {
                status = runningStatus;
                in.seekg(in.tellg() - 1);
            } else {
                runningStatus = status;
            }

            switch (status & 0xF0) {
                case 0x80: {
                    int ch = status & 0x0F;
                    char d1 = in.get();
                    char d2 = in.get();
                    createMidiEvent(t, tick, delta, MidiEventType::NoteOff, ch, d1, d2);
                    break;
                }
                case 0x90: {
                    int ch = status & 0x0F;
                    char d1 = in.get();
                    char d2 = in.get();
                    if (d2 != 0) {
                        createMidiEvent(t, tick, delta, MidiEventType::NoteOn, ch, d1, d2);
                    } else {
                        createMidiEvent(t, tick, delta, MidiEventType::NoteOff, ch, d1, 0);
                    }
                    break;
                }
                case 0xA0: {
                    int ch = status & 0x0F;
                    char d1 = in.get();
                    char d2 = in.get();
                    createMidiEvent(t, tick, delta, MidiEventType::NoteAftertouch, ch, d1, d2);
                    break;
                }
                case 0xB0: {
                    int ch = status & 0x0F;
                    char d1 = in.get();
                    char d2 = in.get();
                    createMidiEvent(t, tick, delta, MidiEventType::Controller, ch, d1, d2);
                    break;
                }
                case 0xC0: {
                    int ch = status & 0x0F;
                    char d1 = in.get();
                    createMidiEvent(t, tick, delta, MidiEventType::ProgramChange, ch, d1, 0);
                    break;
                }
                case 0xD0: {
                    int ch = status & 0x0F;
                    char d1 = in.get();
                    createMidiEvent(t, tick, delta, MidiEventType::ChannelAftertouch, ch, d1, 0);
                    break;
                }
                case 0xE0: {
                    int ch = status & 0x0F;
                    char d1 = in.get();
                    char d2 = in.get();
                    int pitch = ((d2 & 0x7F) << 7) | (d1 & 0x7F);
                    createMidiEvent(t, tick, delta, MidiEventType::PitchBend, ch, pitch, 0);
                    break;
                }
                case 0xF0:
                    std::vector<unsigned char> data;
                    int lenght = 0;
                    switch (status) {
                        case 0xF0:
                        case 0xF7:
                            lenght = readVariableLengthQuantity(&in) + 1;
                            data.push_back(status);
                            for (int i=0; i<lenght -1; i++) {
                                data.push_back(in.get());
                            }
                            createSysExEvent(t, tick, delta, data);
                            break;
                        case 0xFF:
                            char number = in.get();
                            lenght = readVariableLengthQuantity(&in);
                            for (int i=0; i<lenght; i++) {
                                data.push_back(in.get());
                            }
                            createMetaEvent(t, tick, delta, number, data);
                            break;
                    }
                    break;
            }
        }

    }

    std::sort(fEvents.begin(), fEvents.end(), eventTickCompare);
    std::sort(fTempoEvent.begin(), fTempoEvent.end(), eventTickCompare);
    //fEvents.sort(eventTickCompare);
    //fTempoEvent.sort(eventTickCompare);
    in.close();

    return true;
}

MidiEvent* MidiFile::createMidiEvent(int track, uint32_t tick, uint32_t delta, MidiEventType evType, int ch, int data1, int data2) {
    MidiEvent *e = new MidiEvent();
    e->setTrack(track);
    e->setTick(tick);
    e->setDelta(delta);
    e->setEventType(evType);
    e->setChannel(ch);
    e->setData1(data1);
    e->setData2(data2);
    fEvents.push_back(e);

    return e;
}

MidiEvent* MidiFile::createMetaEvent(int track, uint32_t tick, uint32_t delta, int number, std::vector<unsigned char> data) {
    MidiEvent *me = new MidiEvent();
    me->setTrack(track);
    me->setTick(tick);
    me->setDelta(delta);
    me->setEventType(MidiEventType::Meta);
    me->setMetaType(number);
    me->setData(data);
    fEvents.push_back(me);
    if (me->metaEventType() == MidiMetaType::SetTempo)
        fTempoEvent.push_back(me);

    return me;
}

MidiEvent* MidiFile::createSysExEvent(int track, uint32_t tick, uint32_t delta, std::vector<unsigned char> data) {
    MidiEvent *e = new MidiEvent();
    e->setTrack(track);
    e->setTick(tick);
    e->setDelta(delta);
    e->setEventType(MidiEventType::SysEx);
    e->setData(data);
    fEvents.push_back(e);

    return e;
}


