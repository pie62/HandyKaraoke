//
// Created by noob on 16/5/2560.
//

#include "MidiEvent.h"

MidiEvent::MidiEvent() {
    eTick = 0;
    eDelta = 0;
    eTrack = 0;
    eChannel = -1;
    eData1 = 0;
    eData2 = 0;
    eType = MidiEventType::None;
    mType = MidiMetaType::Invalid;
}

MidiEvent::~MidiEvent() {
    mData.clear();
}

void MidiEvent::setMetaType(int mNumber) {
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

int32_t MidiEvent::message() {
    if ((eType != MidiEventType::None)
            && (eType != MidiEventType::Meta) &&
            (eType != MidiEventType::SysEx) ) {
        return static_cast<int32_t>(eType) + eChannel | eData1 << 8 | eData2 << 16;
    } else {
        return 0;
    }
}

float MidiEvent::tempoBpm()
{
    int32_t midi_tempo = 0;

    if ((eType != MidiEventType::Meta) || (mType != MidiMetaType::SetTempo)) {
            return 0;
    }

    midi_tempo = (mData[0] << 16) | (mData[1] << 8) | mData[2];
    return (float)(60000000.0 / midi_tempo);
}
