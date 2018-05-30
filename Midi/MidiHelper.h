#ifndef MIDIHELPER_H
#define MIDIHELPER_H

#include <QObject>
#include <bass.h>

enum class InstrumentType;
enum class SpeakerType;

typedef struct SignatureBeat
{
    int nBeat;
    int nBeatInBar;
    SignatureBeat() : nBeat(0) , nBeatInBar(4) {}
} SignatureBeat;

class MidiHelper
{
public:
    MidiHelper();

    static QStringList GMInstrumentNumberNames();
    static QStringList drumKitNumberNames();
    static QStringList snareNumberName();

    static QStringList GMInstrumentNames();
    static QStringList drumKitNames();
    static QStringList snareName();

    static QStringList instrumentGroupNames();

    static InstrumentType getInstrumentDrumType(int drumNote);
    static InstrumentType getInstrumentType(int instNumber);
};

enum class InstrumentType
{
    Piano, // 0 - 7
    Organ, // 16 - 20
    Accordion, // 21 - 23
    ChromaticPercussion, // 8 - 15
    Percussive, // 112 - 119
    Bass, // 32 - 39
    AcousticGuitarNylon, // 24
    AcousticGuitarSteel, // 25
    ElectricGuitarJazz, // 26
    ElectricGuitarClean, // 27 - 28
    OverdrivenGuitar, // 29
    DistortionGuitar, // 30
    HarmonicsGuitar, // 31
    Trumpet, // 56
    Brass, // 57 - 61
    SynthBrass, // 62 - 63
    Saxophone, // 64 - 67
    Reed, // 68 - 71
    Pipe, // 72 - 79
    Strings, // 40 - 47
    Ensemble, // 48 - 55
    SynthLead, // 80 - 87
    SynthPad, // 88 - 95
    SynthEffects, // 96 - 103
    Ethnic, // 104 - 111
    SoundEffects, // 120 - 127

    BassDrum,
    Snare,
    SideStick,
    HighTom,
    MidTom,
    LowTom,
    Hihat,
    Cowbell,
    CrashCymbal,
    RideCymbal,
    Bongo,
    Conga,
    Timbale,
    SmallCupShapedCymbals,
    ThaiChap,
    PercussionEtc,

    BusGroup1,
    BusGroup2,
    BusGroup3,
    BusGroup4,
    BusGroup5,
    BusGroup6,
    BusGroup7,
    BusGroup8,
    BusGroup9,
    BusGroup10,
    BusGroup11,
    BusGroup12,
    BusGroup13,
    BusGroup14,
    BusGroup15,
    BusGroup16
};

enum class SpeakerType
{
    SpeakerDefault          = -1,

    SpeakerFrontStereo      = 0,
    SpeakerFrontLeft        = 1,
    SpeakerFrontRight       = 2,

    SpeakerRearStereo       = 3,
    SpeakerRearLeft         = 4,
    SpeakerRearRight        = 5,

    SpeakerCenterStereo     = 6,
    SpeakerCenterMono       = 7,
    SpeakerSubwooferMono    = 8,

    SpeakerSideStereo       = 9,
    SpeakerSideLeft         = 10,
    SpeakerSideRight        = 11
};

#endif // MIDIHELPER_H
