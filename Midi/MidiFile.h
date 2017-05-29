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
    int resorution() { return fResorution; }
    DivisionType divisionType() { return fDivision; }
    std::vector<MidiEvent*> events() { return fEvents; }
    std::vector<MidiEvent*> tempoEvents() { return fTempoEvent; }
    int bpm();

    void clear();
    bool read(std::string filename);

    MidiEvent* createMidiEvent(int track, uint32_t tick, uint32_t delta, MidiEventType evType, int ch, int data1, int data2);
    MidiEvent* createMetaEvent(int track, uint32_t tick, uint32_t delta, int number, std::vector<unsigned char> data);
    MidiEvent* createSysExEvent(int track, uint32_t tick, uint32_t delta, std::vector<unsigned char> data);

private:
    int fFormatType;
    int fNumOfTracks;
    int fResorution;
    DivisionType fDivision;
    std::vector<MidiEvent*> fEvents;
    std::vector<MidiEvent*> fTempoEvent;
};


#endif //MIDI_MIDIFILE_H
