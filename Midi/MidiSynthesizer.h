#ifndef MIDISYNTHESIZER_H
#define MIDISYNTHESIZER_H

/*
    Midi stream custom to use 32 channel

        channel 0  - 15 is default intruments
        channel 16 - 31 is a custom drum.

*/

#include <bass.h>
#include <bassmidi.h>

#include "BASSFX/Equalizer31BandFX.h"
#include <BASSFX/ReverbFX.h>

#include <vector>
#include <string>


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

    static std::vector<std::string> audioDevices();
    static bool isSoundFontFile(std::string sfile);

    // Fx ----------------------
    Equalizer31BandFX* equalizer31BandFX() { return eq; }
    ReverbFX* reverbFX() { return reverb; }
    // ------------------------------------------

private:
    HSTREAM stream;
    std::vector<HSOUNDFONT> synth_HSOUNDFONT;
    std::vector<std::string> sfFiles;
    std::vector<int> intmSf;

    // FX
    Equalizer31BandFX *eq;
    ReverbFX *reverb;
    // ---------------------------

    float synth_volume = 1.0f;
    bool openned = false;

    int ourtDev = -1;

    void setSfToStream();
    int getDrumChannelFromNote(int drumNote);
};

#endif // MIDISYNTHESIZER_H
