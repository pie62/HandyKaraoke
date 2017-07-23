//
// Created by noob on 16/5/2560.
//

#ifndef MIDI_MIDIFILE_H
#define MIDI_MIDIFILE_H

#include "MidiEvent.h"
#include <istream>
#include <list>

class MidiFile {
public:
    enum DivisionType {
        Invalid = -1,
        PPQ = 0,
        SMPTE24 = -24,
        SMPTE25 = -25,
        SMPTE30DROP = -29,
        SMPTE30 = -30
    };

    MidiFile();
    ~MidiFile();

    int formatType() { return fFormatType; }
    int numberOfTracks() { return fNumOfTracks; }
    int resorution() { return fResolution; }
    int bpm();

    DivisionType divisionType() { return fDivision; }
    std::vector<MidiEvent*> events() { return fEvents; }
    std::vector<MidiEvent*> tempoEvents() { return fTempoEvents; }
    std::vector<MidiEvent*> controllerEvents() { return fControllerEvents; }
    std::vector<MidiEvent*> programChangeEvents() { return fProgramChangeEvents; }
    std::vector<MidiEvent*> timeSignatureEvents() { return fTimeSignatureEvents; }
    std::vector<MidiEvent*> controllerAndProgramEvents();

    void clear();
    bool read(const std::string &filename, bool seekFileChunkID = false);

    MidiEvent* createMidiEvent(int track, uint32_t tick, uint32_t delta, MidiEventType evType, int ch, int data1, int data2);
    MidiEvent* createMetaEvent(int track, uint32_t tick, uint32_t delta, int number, std::vector<unsigned char> data);
    MidiEvent* createSysExEvent(int track, uint32_t tick, uint32_t delta, std::vector<unsigned char> data);

    float    beatFromTick(uint32_t tick);
    float    timeFromTick(uint32_t tick);
    uint32_t tickFromTime(float time);
    uint32_t tickFromTimeMs(float msTime);


private:
    int fFormatType;
    int fNumOfTracks;
    int fResolution;
    DivisionType fDivision;
    std::vector<MidiEvent*> fEvents;
    std::vector<MidiEvent*> fTempoEvents;
    std::vector<MidiEvent*> fControllerEvents;
    std::vector<MidiEvent*> fProgramChangeEvents;
    std::vector<MidiEvent*> fTimeSignatureEvents;
};


#endif //MIDI_MIDIFILE_H
