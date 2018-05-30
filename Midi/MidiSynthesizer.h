#ifndef MIDISYNTHESIZER_H
#define MIDISYNTHESIZER_H

#include <bass.h>
#include <bassmidi.h>
#include <bassmix.h>
#include <bass_fx.h>

#include "BASSFX/FX.h"
#include "BASSFX/Equalizer31BandFX.h"
#include "BASSFX/ReverbFX.h"
#include "BASSFX/ChorusFX.h"

#include "Midi/MidiHelper.h"

#include <QObject>
#include <QMap>


struct Instrument
{
    InstrumentType type;
    bool mute;
    bool solo;
    bool enable;
    int volume;
    int bus;
    QList<FX*> FXs;
};

typedef struct
{
    unsigned int uniqueID;
    QString vstName;
    QString vstvendor;
    QString vstPath;
} VSTNamePath;

class MidiSynthesizer : public QObject
{
    Q_OBJECT

public:
    MidiSynthesizer(QObject *parent = nullptr);
    ~MidiSynthesizer();

    bool isOpened() { return openned; }
    QStringList soundfontFiles() { return sfFiles; }

    bool open();
    void close();

    int outPutDevice();
    bool setOutputDevice(int dv);
    void setSoundFonts(QStringList &soundfonsFiles);
    void setVolume(float vol);
    float volume() { return synth_volume; }

    float soundfontVolume(int sfIndex);
    void setSoundfontVolume(int sfIndex, float sfvl);

    // std::vector<int> size 129
    //      1-128 all intrument
    //      129 is drum
    bool setMapSoundfontIndex(QList<int> intrumentSfIndex, QList<int> drumSfIndex);
    QList<int> getMapSoundfontIndex() { return instmSf; }
    QList<int> getDrumMapSfIndex() { return drumSf; }


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
    QMap<InstrumentType, Instrument> instrumentMap() { return instMap; }
    Instrument instrument(InstrumentType t) { return instMap.value(t); }
    int busGroup(InstrumentType t);
    int volume(InstrumentType t);
    bool isMute(InstrumentType t);
    bool isSolo(InstrumentType t);
    void setBusGroup(InstrumentType t, int group);
    void setVolume(InstrumentType t, int volume);
    void setMute(InstrumentType t, bool m);
    void setSolo(InstrumentType t, bool s);


    static std::vector<std::string> audioDevices();
    static bool isSoundFontFile(const QString &sfile);

    // Fx ----------------------
    Equalizer31BandFX* equalizer31BandFX() { return eq; }
    ReverbFX* reverbFX() { return reverb; }
    ChorusFX* chorusFX() { return chorus; }

    QMap<uint, VSTNamePath> VSTList() { return _vstList; }
    void setVSTList(const QMap<uint, VSTNamePath> &listMap) { _vstList = listMap; }
    // ------------------------------------------


    bool isUseFloattingPoint() { return useFloat; }
    void setUsetFloattingPoint(bool use);

    bool isUseFXRC() { return useFX; }
    void setUseFXRC(bool use);

    HSTREAM getChannelHandle(InstrumentType type);

    FX* addFX(InstrumentType type, DWORD uid);
    bool removeFX(InstrumentType type, int fxIndex);
    void setFXBypass(InstrumentType type, int fxIndex, bool state);
    QList<uint> fxUids(InstrumentType type);
    QList<bool> fxBypass(InstrumentType type);
    QList<int> fxProgram(InstrumentType type);
    QList<QList<float>> fxParams(InstrumentType type);

    // Speakers
    static DWORD getSpeakerFlag(SpeakerType Speaker);
    void setSpeaker(InstrumentType type, int device, SpeakerType Speaker);

signals:
    void noteOnSended(InstrumentType t, int bus, int ch, int note, int velocity);

private:
    const int HANDLE_STREAM_COUNT = 58;
    const int HANDLE_MIDI_COUNT = 42;
    const int HANDLE_BUS_COUNT = 16;

    HSTREAM mixHandle;
    QMap<InstrumentType, HSTREAM> handles;
    QList<HSOUNDFONT> synth_HSOUNDFONT;
    QStringList sfFiles;
    QList<int> instmSf;
    QList<int> drumSf;
    QMap<InstrumentType, Instrument> instMap;
    InstrumentType chInstType[16];

    // FX
    Equalizer31BandFX *eq;
    ReverbFX *reverb;
    ChorusFX *chorus;

    // unique ID,  VSTNamePAth
    QMap<uint, VSTNamePath> _vstList;
    // ---------------------------

    float synth_volume = 1.0f;
    bool openned = false;
    bool useSolo = false;

    int outDev = 1;
    bool useFloat = true;
    bool useFX = false;

    DWORD RPNType = 0;

    void sendToAllMidiStream(int ch, DWORD eventType, DWORD param);
    void setSfToStream();
    void calculateEnable();
    HSTREAM getDrumHandleFromNote(int drumNote);
    std::vector<int> getChannelsFromType(InstrumentType t);
};

#endif // MIDISYNTHESIZER_H
