//
// Created by noob on 16/5/2560.
//

#ifndef MIDI_MIDIEVENT_H
#define MIDI_MIDIEVENT_H

#include <cstdint>
#include <vector>

enum class MidiEventType {
    NoteOff = 0x80,
    NoteOn = 0x90,
    NoteAftertouch = 0xA0,
    Controller = 0xB0,
    ProgramChange = 0xC0,
    ChannelAftertouch = 0xD0,
    PitchBend = 0xE0,
    Meta = 0xFF,
    SysEx = 0xF7,
    None = 0
};

enum class MidiMetaType {
    SequenceNumber = 0x00,
    TextEvent = 0x01,
    CopyrightNotice = 0x02,
    SequenceTrackName = 0x03,
    InstrumentName = 0x04,
    Lyrics = 0x05,
    Marker = 0x06,
    CuePoint = 0x07,
    MIDIChannelPrefix = 0x20,
    EndOfTrack = 0x2F,
    SetTempo = 0x51,
    SMPTEOffset = 0x54,
    TimeSignature = 0x58,
    KeySignature = 0x59,
    SequencerSpecific = 0x7F,
    Invalid = 0xFF
};


class MidiEvent {
public:
    MidiEvent();
    ~MidiEvent();
    MidiEvent &operator = (const MidiEvent &e);

    void setTick(uint32_t t)        { eTick = t; }
    void setDelta(uint32_t d)       { eDelta = d; }
    void setTrack(int t)            { eTrack = t; }
    void setChannel(int ch)         { eChannel = ch; }
    void setData1(int d1)           { eData1 = d1; }
    void setData2(int d2)           { eData2 = d2; }
    void setEventType(MidiEventType et) { eType = et; }
    void setData(std::vector<unsigned char> md) { mData = md; }
    void setMetaType(MidiMetaType t)  { mType = t; }
    void setMetaType(int mNumber);

    int32_t         message();
    uint32_t        tick() const           { return eTick; }
    uint32_t        delta() const          { return eDelta; }
    int             track() const          { return eTrack; }
    int             channel() const        { return eChannel; }
    int             data1() const          { return eData1; }
    int             data2() const          { return eData2; }
    MidiEventType   eventType() const      { return eType; }
    MidiMetaType    metaEventType() const  { return mType; }
    std::vector<unsigned char> data() const{ return mData; }

    float tempoBpm();

private:
    uint32_t eTick;
    uint32_t eDelta;
    int eTrack;
    int eChannel;
    int eData1;
    int eData2;
    MidiEventType eType;
    MidiMetaType mType;
    std::vector<unsigned char> mData; // Meta, SysEx
};


#endif //MIDI_MIDIEVENT_H
