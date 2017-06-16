#include "MidiSynthesizer.h"

#include <cstring>
#include <thread>
#include <QDebug>

MidiSynthesizer::MidiSynthesizer()
{
    BASS_Init(-1, 44100, 0, NULL, NULL);

    DWORD flags = BASS_SAMPLE_FLOAT|BASS_MIDI_SINCINTER;
    stream = BASS_MIDI_StreamCreate(16, flags, 0);

    BASS_SetConfig(
        BASS_CONFIG_UPDATEPERIOD,
        5
    );

    auto concurentThreadsSupported = std::thread::hardware_concurrency();
    float nVoices = (concurentThreadsSupported > 1) ? 500 : 256;
    qDebug() << concurentThreadsSupported;

    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_MIDI_VOICES, nVoices);
    BASS_ChannelPlay(stream, false); 

    float v;
    BASS_ChannelGetAttribute(stream, BASS_ATTRIB_MIDI_VOICES, &v);
    qDebug() << v;
    //BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_MIXLEVEL, 50);
}

MidiSynthesizer::~MidiSynthesizer()
{
    BASS_ChannelStop(stream);

    for (HSOUNDFONT f : synth_HSOUNDFONT)
        BASS_MIDI_FontFree(f);

    synth_HSOUNDFONT.clear();
    synth_BASS_MIDI_FONT.clear();

    BASS_StreamFree(stream);
    BASS_Free();
}

bool MidiSynthesizer::setOutputDevice(int dv)
{
    return BASS_SetDevice(dv);
}

void MidiSynthesizer::setSoundFonts(std::vector<std::__cxx11::string> &soundfonts)
{
    for (HSOUNDFONT f : synth_HSOUNDFONT) {
        BASS_MIDI_FontFree(f);
    }
    synth_HSOUNDFONT.clear();
    synth_BASS_MIDI_FONT.clear();

    int count = 0;
    for (int i=0; i<soundfonts.size(); i++) {
        HSOUNDFONT f = BASS_MIDI_FontInit(soundfonts[i].c_str(), BASS_MIDI_FONT_NOFX);
        if (f) {
            count++;
            synth_HSOUNDFONT.push_back(f);

            BASS_MIDI_FONT font;
            font.font = f;
            font.preset = -1;
            font.bank = 0;
            synth_BASS_MIDI_FONT.push_back(font);
        }
    }

    if (synth_BASS_MIDI_FONT.size() > 0) {
        BASS_MIDI_StreamSetFonts(0, &synth_BASS_MIDI_FONT[0], 1);
        BASS_MIDI_StreamSetFonts(stream, synth_BASS_MIDI_FONT.data(), synth_BASS_MIDI_FONT.size());
    }

    BASS_MIDI_FontCompact(0);

    BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_DRUM_LEVEL, MAKEWORD(38, 40));
    BASS_MIDI_StreamEvent(stream, 9, MIDI_EVENT_DRUM_LEVEL, MAKEWORD(40, 40));
}

void MidiSynthesizer::setVolume(float vol)
{
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, vol);
}

void MidiSynthesizer::reset()
{
    /*float vol, voice;
    BASS_ChannelGetAttribute(stream, BASS_ATTRIB_VOL, &vol);
    BASS_ChannelGetAttribute(stream, BASS_ATTRIB_MIDI_VOICES, &voice);

    BASS_ChannelStop(stream);
    BASS_StreamFree(stream);

    stream = BASS_MIDI_StreamCreate(16, BASS_SAMPLE_FLOAT|BASS_MIDI_NOFX, 0);
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, vol);
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_MIDI_VOICES, voice);
    BASS_ChannelPlay(stream, FALSE);*/


    //BASS_ChannelStop(stream);
    //BASS_ChannelPlay(stream, false);
}

void MidiSynthesizer::sendNoteOff(int ch, int note, int velocity)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_NOTE, note);
}

void MidiSynthesizer::sendNoteOn(int ch, int note, int velocity)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_NOTE, MAKEWORD(note, velocity));
}

void MidiSynthesizer::sendNoteAftertouch(int ch, int note, int value)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_KEYPRES, MAKEWORD(note, value));
}

void MidiSynthesizer::sendController(int ch, int number, int value)
{
    switch (number) {
    case 0:
         BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_BANK, value);
        break;
    case 1:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_MODULATION, value);
        break;
    case 5:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_PORTATIME, value);
        break;
    case 7:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_VOLUME, value);
        break;
    case 10:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_PAN, value);
        break;
    case 11:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_EXPRESSION, value);
        break;
    case 32:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_BANK_LSB, value);
        break;
    case 64:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_SUSTAIN, value);
        break;
    case 65:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_PORTAMENTO, value);
        break;
    case 66:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_SOSTENUTO, value);
        break;
    case 67:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_SOFT, value);
        break;
    case 71:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_RESONANCE, value);
        break;
    case 72:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_RELEASE, value);
        break;
    case 73:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_ATTACK, value);
        break;
    case 74:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_CUTOFF, value);
        break;
    case 75:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_DECAY, value);
        break;
    case 84:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_PORTANOTE, value);
        break;
    case 91:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_REVERB, value);
        break;
    case 93:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_CHORUS, value);
        break;
    case 94:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_USERFX, value);
        break;
    case 120:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_SOUNDOFF, 0);
        break;
    case 121:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_RESET, 0);
        break;
    case 123:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_NOTESOFF, 0);
        break;
    case 126:
    case 127:
        BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_MODE, value);
        break;
    default:
        break;
    }
}

void MidiSynthesizer::sendProgramChange(int ch, int number)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_PROGRAM, number);
}

void MidiSynthesizer::sendChannelAftertouch(int ch, int value)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_CHANPRES, value);
}

void MidiSynthesizer::sendPitchBend(int ch, int value)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_PITCH, value);
}

void MidiSynthesizer::sendAllNotesOff(int ch)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_NOTESOFF, 0);
}

void MidiSynthesizer::sendAllNotesOff()
{
    for (int i=0; i<16; i++) {
        sendAllNotesOff(i);
    }
}

void MidiSynthesizer::sendResetAllControllers(int ch)
{
    BASS_MIDI_StreamEvent(stream, ch, MIDI_EVENT_RESET, 0);
}

void MidiSynthesizer::sendResetAllControllers()
{
    for (int i=0; i<16; i++) {
        sendResetAllControllers(i);
    }
}
