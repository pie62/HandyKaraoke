#ifndef MIDIHELPER_H
#define MIDIHELPER_H

#include <QObject>

enum class InstrumentType;

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
    LowTom,
    MidTom,
    HighTom,
    Hihat,
    Cowbell,
    CrashCymbal,
    RideCymbal,
    Bongo,
    Conga,
    Timbale,
    SmallCupShapedCymbals,
    ChineseCymbal,
    PercussionEtc
};

#endif // MIDIHELPER_H
