#include "MidiEvent.h"

MidiEvent::MidiEvent()
{
}

MidiEvent::~MidiEvent()
{
    mData.clear();
}

int32_t MidiEvent::message()
{
    if ((eType != MidiEventType::Invalid)
            && (eType != MidiEventType::Meta)
            && (eType != MidiEventType::SysEx) ) {
        return static_cast<int32_t>(eType) + eChannel | eData1 << 8 | eData2 << 16;
    } else {
        return 0;
    }
}

float MidiEvent::bpm()
{
    if ((eType != MidiEventType::Meta) || (mType != MidiMetaType::SetTempo)) {
        return 0;
    }

    unsigned char* buffer = (unsigned char*)mData.constData();
    int32_t midi_tempo = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
    return (float)(60000000.0 / midi_tempo);
}

void MidiEvent::setMetaType(int mNumber)
{
    switch (mNumber) {
        case 0:   mType = MidiMetaType::SequenceNumber; break;
        case 1:   mType = MidiMetaType::TextEvent; break;
        case 2:   mType = MidiMetaType::CopyrightNotice; break;
        case 3:   mType = MidiMetaType::SequenceTrackName; break;
        case 4:   mType = MidiMetaType::InstrumentName; break;
        case 5:   mType = MidiMetaType::Lyrics; break;
        case 6:   mType = MidiMetaType::Marker; break;
        case 7:   mType = MidiMetaType::CuePoint; break;
        case 32:  mType = MidiMetaType::MIDIChannelPrefix; break;
        case 47:  mType = MidiMetaType::EndOfTrack; break;
        case 81:  mType = MidiMetaType::SetTempo; break;
        case 84:  mType = MidiMetaType::SMPTEOffset; break;
        case 88:  mType = MidiMetaType::TimeSignature; break;
        case 89:  mType = MidiMetaType::KeySignature; break;
        case 127: mType = MidiMetaType::SequencerSpecific; break;
        default:  mType = MidiMetaType::Invalid; break;
    }
}

MidiEvent &MidiEvent::operator =(const MidiEvent &e)
{
    mData.clear();

    eTick       = e.tick();
    eDelta      = e.delta();
    eTrack      = e.track();
    eChannel    = e.channel();
    eData1      = e.data1();
    eData2      = e.data2();
    eType       = e.eventType();
    mType       = e.metaEventType();
    mData       = e.data();

    return *this;
}
