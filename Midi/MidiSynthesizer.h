#ifndef MIDISYNTHESIZER_H
#define MIDISYNTHESIZER_H

/*
    Midi stream custom to use 32 channel

        channel 0  - 15 is default intruments
        channel 16 - 31 is a custom drum.

*/

#include <bass.h>
#include <bassmidi.h>
#include <bass_fx.h>

#include "BASSFX/Equalizer24BandFX.h"
#include "BASSFX/ReverbFX.h"
#include "BASSFX/ChorusFX.h"

#include "Midi/MidiHelper.h"

#include <vector>
#include <string>
#include <map>


struct Instrument
{
    InstrumentType type;
    bool mute;
    bool solo;
    bool enable;
    int mixlevel;
};


class MidiSynthesizer
{
public:
    MidiSynthesizer();
    ~MidiSynthesizer();

    bool isOpened() { return openned; }
    std::vector<std::string> soundfontFiles() { return sfFiles; }

    bool open();
    void close();

    int outPutDevice();
    bool setOutputDevice(int dv);
    void setSoundFonts(std::vector<std::string> &soundfonsFiles);
    void setVolume(float vol);
    float volume() { return synth_volume; }

    float soundfontVolume(int sfIndex);
    void setSoundfontVolume(int sfIndex, float sfvl);

    // std::vector<int> size 129
    //      1-128 all intrument
    //      129 is drum
    bool setMapSoundfontIndex(const std::vector<int> &intrumentSfIndex);
    std::vector<int> getMapSoundfontIndex() { return intmSf; }


    void sendNoteOff(int ch, int note, int velocity);
    void sendNoteOn(int ch, int note, int velocity);
    void sendNoteAftertouch(int ch, int note, int value);
    void sendController(int ch, int number, int value);
    void sendProgramChange(int ch, int number);
    void sendChannelAftertouch(int ch, int value);
    void sendPitchBend(int ch, int value);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
    void sendResetAllControllers(int ch);
    void sendResetAllControllers();


    // Instrument Maper
    std::map<InstrumentType, Instrument> instrumentMap() { return instMap; }
    int mixLevel(InstrumentType t);
    bool isMute(InstrumentType t);
    bool isSolo(InstrumentType t);
    void setMixLevel(InstrumentType t, int level);
    void setMute(InstrumentType t, bool m);
    void setSolo(InstrumentType t, bool s);


    static std::vector<std::string> audioDevices();
    static bool isSoundFontFile(std::string sfile);

    // Fx ----------------------
    Equalizer24BandFX* equalizer24BandFX() { return eq; }
    ReverbFX* reverbFX() { return reverb; }
    ChorusFX* chorusFX() { return chorus; }
    // ------------------------------------------

private:
    HSTREAM stream;
    std::vector<HSOUNDFONT> synth_HSOUNDFONT;
    std::vector<std::string> sfFiles;
    std::vector<int> intmSf;
    std::map<InstrumentType, Instrument> instMap;
    InstrumentType chInstType[16];

    // FX
    Equalizer24BandFX *eq;
    ReverbFX *reverb;
    ChorusFX *chorus;
    // ---------------------------

    float synth_volume = 1.0f;
    bool openned = false;
    bool useSolo = false;

    int outDev = -1;

    void setSfToStream();
    void calculateEnable();
    int getDrumChannelFromNote(int drumNote);
    std::vector<int> getChannelsFromType(InstrumentType t);
};

#endif // MIDISYNTHESIZER_H
