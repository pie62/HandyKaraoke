#include "MidiHelper.h"

#include "MidiFile.h"

#include <QtMath>


MidiHelper::MidiHelper()
{

}

int MidiHelper::getNumberBeatInBar(int numerator, int denominator)
{
    int value;
    int d = qPow(2, denominator);
    switch (d)
    {
        case 2:
        case 4:
            value = numerator * 1;
            break;
        case 8:
            value = numerator * 0.5;
            break;
        case 16:
            value = numerator * 0.25;
            break;
        default:
            value = 4;
            break;
    }

    if (value == 16 || value == 8)
        value = 4;

    switch (value)
    {
        case 16:
        case 8:
            value = 4;
            break;
        case 12:
            value = 6;
            break;
    }

    return value;
}

QList<SignatureBeat> MidiHelper::calculateBeats(MidiFile *midi)
{
    QList<SignatureBeat> beats;
    uint32_t t = midi->events().back()->tick();
    ushort bCount = midi->beatFromTick(t);

    for (MidiEvent *evt : midi->timeSignatureEvents())
    {
        int nBeatInBar = getNumberBeatInBar(evt->data()[0], evt->data()[1]);
        if (nBeatInBar <= 0)
            continue;

        SignatureBeat sb;
        sb.nBeat = midi->beatFromTick(evt->tick());
        sb.nBeatInBar = nBeatInBar;
        beats.append(sb);
    }

    return beats;
}

QStringList MidiHelper::GMInstrumentNumberNames()
{
    QStringList gln;
    QStringList gl = GMInstrumentNames();

    for (int i=0; i<gl.count(); i++) {
        QString n = QString::number(i).rightJustified(3, '0');
        n = n + " - " + gl[i];
        gln.append(n);
    }

    return gln;
}

QStringList MidiHelper::drumKitNumberNames()
{
    QStringList dln;
    QStringList dl = drumKitNames();


    for (int i=0; i<dl.count(); i++) {
        QString n = QString::number(i).rightJustified(3, '0');
        n = n + " - " + dl[i];
        dln.append(n);
    }

    return dln;
}

QStringList MidiHelper::snareNumberName()
{
    QStringList sl;
    sl.append("38 - " + snareName()[0]);
    sl.append("40 - " + snareName()[1]);
    return sl;
}

QStringList MidiHelper::GMInstrumentNames()
{
    QStringList gmList;
    gmList << "Acoustic Grand Piano" <<
        "Bright Acoustic Piano" <<
        "Electric Grand Piano" <<
        "Honky-tonk Piano" <<
        "Electric Piano 1" <<
        "Electric Piano 2" <<
        "Harpsichord" <<
        "Clavi" <<
        "Celesta" <<
        "Glockenspiel" <<
        "Music Box" <<
        "Vibraphone" <<
        "Marimba" <<
        "Xylophone" <<
        "Tubular Bells" <<
        "Dulcimer" <<
        "Drawbar Organ" <<
        "Percussive Organ" <<
        "Rock Organ" <<
        "Church Organ" <<
        "Reed Organ" <<
        "Accordion" <<
        "Harmonica" <<
        "Tango Accordion" <<
        "Acoustic Guitar (nylon)" <<
        "Acoustic Guitar (steel)" <<
        "Electric Guitar (jazz)" <<
        "Electric Guitar (clean)" <<
        "Electric Guitar (muted)" <<
        "Overdriven Guitar" <<
        "Distortion Guitar" <<
        "Guitar harmonics" <<
        "Acoustic Bass" <<
        "Electric Bass (finger)" <<
        "Electric Bass (pick)" <<
        "Fretless Bass" <<
        "Slap Bass 1" <<
        "Slap Bass 2" <<
        "Synth Bass 1" <<
        "Synth Bass 2" <<
        "Violin" <<
        "Viola" <<
        "Cello" <<
        "Contrabass" <<
        "Tremolo Strings" <<
        "Pizzicato Strings" <<
        "Orchestral Harp" <<
        "Timpani" <<
        "String Ensemble 1" <<
        "String Ensemble 2" <<
        "SynthStrings 1" <<
        "SynthStrings 2" <<
        "Choir Aahs" <<
        "Voice Oohs" <<
        "Synth Voice" <<
        "Orchestra Hit" <<
        "Trumpet" <<
        "Trombone" <<
        "Tuba" <<
        "Muted Trumpet" <<
        "French Horn" <<
        "Brass Section" <<
        "SynthBrass 1" <<
        "SynthBrass 2" <<
        "Soprano Sax" <<
        "Alto Sax" <<
        "Tenor Sax" <<
        "Baritone Sax" <<
        "Oboe" <<
        "English Horn" <<
        "Bassoon" <<
        "Clarinet" <<
        "Piccolo" <<
        "Flute" <<
        "Recorder" <<
        "Pan Flute" <<
        "Blown Bottle" <<
        "Shakuhachi" <<
        "Whistle" <<
        "Ocarina" <<
        "Lead 1 (square)" <<
        "Lead 2 (sawtooth)" <<
        "Lead 3 (calliope)" <<
        "Lead 4 (chiff)" <<
        "Lead 5 (charang)" <<
        "Lead 6 (voice)" <<
        "Lead 7 (fifths)" <<
        "Lead 8 (bass + lead)" <<
        "Pad 1 (new age)" <<
        "Pad 2 (warm)" <<
        "Pad 3 (polysynth)" <<
        "Pad 4 (choir)" <<
        "Pad 5 (bowed)" <<
        "Pad 6 (metallic)" <<
        "Pad 7 (halo)" <<
        "Pad 8 (sweep)" <<
        "FX 1 (rain)" <<
        "FX 2 (soundtrack)" <<
        "FX 3 (crystal)" <<
        "FX 4 (atmosphere)" <<
        "FX 5 (brightness)" <<
        "FX 6 (goblins)" <<
        "FX 7 (echoes)" <<
        "FX 8 (sci-fi)" <<
        "Sitar" <<
        "Banjo" <<
        "Shamisen" <<
        "Koto" <<
        "Kalimba" <<
        "Bag pipe" <<
        "Fiddle" <<
        "Shanai" <<
        "Tinkle Bell" <<
        "Agogo" <<
        "Steel Drums" <<
        "Woodblock" <<
        "Taiko Drum" <<
        "Melodic Tom" <<
        "Synth Drum" <<
        "Reverse Cymbal" <<
        "Guitar Fret Noise" <<
        "Breath Noise" <<
        "Seashore" <<
        "Bird Tweet" <<
        "Telephone Ring" <<
        "Helicopter" <<
        "Applause" <<
        "Gunshot";

    return gmList;
}

QStringList MidiHelper::drumKitNames()
{
    QStringList dList;

    for (int i=0; i<128; i++) {
        switch (i) {
        case 0:
            dList << "Standard";
            break;
        case 8:
            dList << "Room";
            break;
        case 16:
            dList << "Power";
            break;
        case 24:
            dList << "Electronic";
            break;
        case 25:
            dList << "TR-808";
            break;
        case 32:
            dList << "Jazz";
            break;
        case 40:
            dList << "Brush";
            break;
        case 48:
            dList << "Orchestra";
            break;
        case 56:
            dList << "Sound FX";
            break;
        default:
            dList << "Drum Kit";
            break;
        }
    }

    return dList;
}

QStringList MidiHelper::snareName()
{
    QStringList slist;
    slist.append("Acoustic Snare");
    slist.append("Electric Snare");
    return slist;
}

QStringList MidiHelper::instrumentGroupNames()
{
    QStringList gn;

    gn << "Piano" << "Chromatic Percussion" << "Organ" << "Guitar" << "Bass"
       << "Strings" << "Ensemble" << "Brass" << "Reed" << "Pipe" << "Synth Lead"
       << "Synth Pad" << "Synth Effects" << "Ethnic" << "Percussive"
       << "Sound effects";

    return gn;
}

InstrumentType MidiHelper::getInstrumentDrumType(int drumNote)
{
    InstrumentType t;

    switch (drumNote) {

    // Bass drum
    case 35:
    case 36: t = InstrumentType::BassDrum; break;

    // snare
    case 38:
    case 40: t = InstrumentType::Snare; break;

    // Side Stick/Rimshot
    case 37: t = InstrumentType::SideStick; break;

    // Low Tom
    case 41:
    case 43: t = InstrumentType::LowTom; break;

    // Mid Tom
    case 45:
    case 47: t = InstrumentType::MidTom; break;

    // High Tom
    case 48:
    case 50: t = InstrumentType::HighTom; break;

    // Hi-hat
    case 42:
    case 44:
    case 46: t = InstrumentType::Hihat; break;

    // Cowbell
    case 56: t = InstrumentType::Cowbell; break;

    // Crash Cymbal ( 55 = Splash Cymbal )
    case 49:
    case 52:
    case 55:
    case 57: t = InstrumentType::CrashCymbal; break;

    // Ride Cymbal
    case 51:
    case 59: t = InstrumentType::RideCymbal; break;

    // Bongo
    case 60:
    case 61: t = InstrumentType::Bongo; break;

    // Conga
    case 62:
    case 63:
    case 64: t = InstrumentType::Conga; break;

    // Timbale
    case 65:
    case 66: t = InstrumentType::Timbale; break;

    // ฉิ่ง
    case 80:
    case 81: t = InstrumentType::SmallCupShapedCymbals; break;

    // Chinese Cymbal ( ฉาบ )
    case 82:
    case 83: t = InstrumentType::ThaiChap; break;

    default:
        t = InstrumentType::PercussionEtc;
        break;
    }

    return t;
}

InstrumentType MidiHelper::getInstrumentType(int instNumber)
{
    InstrumentType t;

    switch (instNumber) {

    case 0: case 1: case 2: case 3:
    case 4: case 5: case 6: case 7:
        t = InstrumentType::Piano;
        break;

    case 8: case 9: case 10: case 11:
    case 12: case 13: case 14: case 15:
        t = InstrumentType::ChromaticPercussion;
        break;

    case 16: case 17: case 18: case 19: case 20:
        t = InstrumentType::Organ;
        break;

    case 21: case 22: case 23:
        t = InstrumentType::Accordion;
        break;

    case 24:
        t = InstrumentType::AcousticGuitarNylon;
        break;

    case 25:
        t= InstrumentType::AcousticGuitarSteel;
        break;

    case 26:
        t = InstrumentType::ElectricGuitarJazz;
        break;

    case 27: case 28:
        t = InstrumentType::ElectricGuitarClean;
        break;

    case 29:
        t = InstrumentType::OverdrivenGuitar;
        break;

    case 30:
        t = InstrumentType::DistortionGuitar;
        break;

    case 31:
        t = InstrumentType::HarmonicsGuitar;
        break;

    case 32: case 33: case 34: case 35:
    case 36: case 37: case 38: case 39:
        t = InstrumentType::Bass;
        break;

    case 40: case 41: case 42: case 43:
    case 44: case 45: case 46: case 47:
        t = InstrumentType::Strings;
        break;

    case 48: case 49: case 50: case 51:
    case 52: case 53: case 54: case 55:
        t = InstrumentType::Ensemble;
        break;

    case 56:
        t = InstrumentType::Trumpet;
        break;

    case 57: case 58: case 59: case 60: case 61:
        t = InstrumentType::Brass;
        break;

    case 62: case 63:
        t = InstrumentType::SynthBrass;
        break;

    case 64: case 65: case 66: case 67:
        t = InstrumentType::Saxophone;
        break;

    case 68: case 69: case 70: case 71:
        t = InstrumentType::Reed;
        break;

    case 72: case 73: case 74: case 75:
    case 76: case 77: case 78: case 79:
        t = InstrumentType::Pipe;
        break;

    case 80: case 81: case 82: case 83:
    case 84: case 85: case 86: case 87:
        t= InstrumentType::SynthLead;
        break;

    case 88: case 89: case 90: case 91:
    case 92: case 93: case 94: case 95:
        t= InstrumentType::SynthPad;
        break;

    case 96: case 97: case 98: case 99:
    case 100: case 101: case 102: case 103:
        t = InstrumentType::SynthEffects;
        break;

    case 104: case 105: case 106: case 107:
    case 108: case 109: case 110: case 111:
        t = InstrumentType::Ethnic;
        break;

    case 112: case 113: case 114: case 115:
    case 116: case 117: case 118: case 119:
        t = InstrumentType::Percussive;
        break;

    default:
        t = InstrumentType::SoundEffects;
        break;
    }

    return t;
}

DWORD MidiHelper::getSpeakerFlag(SpeakerType speaker)
{
    switch (speaker)
    {
    case SpeakerType::Default:
        return 0;

    case SpeakerType::FrontStereo:
        return BASS_SPEAKER_FRONT;
    case SpeakerType::FrontLeft:
        return BASS_SPEAKER_FRONTLEFT;
    case SpeakerType::FrontRight:
        return BASS_SPEAKER_FRONTRIGHT;

    case SpeakerType::RearStereo:
        return BASS_SPEAKER_REAR;
    case SpeakerType::RearLeft:
        return BASS_SPEAKER_REARLEFT;
    case SpeakerType::RearRight:
        return BASS_SPEAKER_REARRIGHT;

    case SpeakerType::CenterStereo:
        return BASS_SPEAKER_CENLFE;
    case SpeakerType::CenterMono:
        return BASS_SPEAKER_CENTER;
    case SpeakerType::SubwooferMono:
        return BASS_SPEAKER_LFE;

    case SpeakerType::SideStereo:
        return BASS_SPEAKER_REAR2;
    case SpeakerType::SideLeft:
        return BASS_SPEAKER_REAR2LEFT;
    case SpeakerType::SideRight:
        return BASS_SPEAKER_REAR2RIGHT;
    }
}

bool MidiHelper::isStereoSpeaker(SpeakerType speaker)
{
    switch (speaker) {
    case SpeakerType::Default:
    case SpeakerType::FrontStereo:
    case SpeakerType::RearStereo:
    case SpeakerType::CenterStereo:
    case SpeakerType::SideStereo:
        return true;
    default:
        return false;
    }
}
