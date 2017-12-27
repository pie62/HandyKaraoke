#ifndef MIDIEVENT_H
#define MIDIEVENT_H

#include <QByteArray>

#ifdef _WIN32
typedef __int32 int32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
#endif

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
    Invalid = 0
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

class MidiEvent
{
public:
    MidiEvent();
    ~MidiEvent();

    int32_t         message();
    uint32_t        tick() const           { return eTick; }
    uint32_t        delta() const          { return eDelta; }
    int             track() const          { return eTrack; }
    int             channel() const        { return eChannel; }
    int             data1() const          { return eData1; }
    int             data2() const          { return eData2; }
    MidiEventType   eventType() const      { return eType; }
    MidiMetaType    metaEventType() const  { return mType; }
    QByteArray      data() const           { return mData; }

    float bpm();

    void setTick(uint32_t t) { eTick = t; }
    void setDelta(uint32_t d){ eDelta = d; }
    void setTrack(int t)    { eTrack = t; }
    void setChannel(int ch) { eChannel = ch; }
    void setData1(int d1)   { eData1 = d1; }
    void setData2(int d2)   { eData2 = d2; }
    void setEventType(MidiEventType et) { eType = et; }
    void setData(const QByteArray &md)  { mData = md; }
    void setMetaType(MidiMetaType t)    { mType = t; }
    void setMetaType(int mNumber);

    MidiEvent &operator = (const MidiEvent &e);

private:
    uint32_t eTick      = 0;
    uint32_t eDelta     = 0;
    int     eTrack      = 0;
    int     eChannel    = -1;
    int     eData1      = 0;
    int     eData2      = 0;
    MidiEventType   eType = MidiEventType::Invalid;
    MidiMetaType    mType = MidiMetaType::Invalid;
    QByteArray      mData; // Meta, SysEx
};

#endif // MIDIEVENT_H
