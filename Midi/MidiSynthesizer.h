#ifndef MIDISYNTHESIZER_H
#define MIDISYNTHESIZER_H

#include <QObject>
#include <QMap>

#include <bass.h>
#include <bassmidi.h>
#include <bassmix.h>
#include <bass_fx.h>

#include "Midi/MidiHelper.h"
#include "BASSFX/FX.h"
#include "BASSFX/Equalizer31BandFX.h"
#include "BASSFX/Chorus2FX.h"
#include "BASSFX/Reverb2FX.h"

#define SF_PRESET_COUNT 11

typedef struct
{
    DWORD handle;
    Equalizer31BandFX *eq;
    Chorus2FX *chorus;
    Reverb2FX *reverb;
} MixerHandle;

typedef struct
{
    InstrumentType type;
    bool mute;
    bool solo;
    bool enable;
    int volume;
    int bus;
    int vsti;
    int device;
    SpeakerType speaker;
    QList<FX*> FXs;
} Instrument;

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

    bool open();
    void close();

    int defaultDevice();
    bool setDefaultDevice(int dv);
    void setVolume(float vol);
    float volume() { return synth_volume; }

    QStringList soundfontFiles() { return sfFiles; }
    bool addSoundfont(const QString &sfFile);
    void removeSoundfont(int sfIndex);
    void swapSoundfont(int sfIndex, int toIndex);
    float soundfontVolume(int sfIndex);
    void setSoundfontVolume(int sfIndex, float sfvl);
    void compactSoundfont();
    bool isLoadAllSoundfont() { return sfLoadAll; }
    void setLoadAllSoundfont(bool loadAll);

    // std::vector<int> size 129
    //      1-128 all intrument
    //      129 is drum
    bool setMapSoundfontIndex(int presetIndex, QList<int> intrumentSfIndex, QList<int> drumSfIndex);
    void setSoundfontPresets(int presetIndex);
    int soundfontPresets() { return sfPreset; }
    QList<int> getMapSoundfontIndex(int presetIndex) { return instmSf[presetIndex]; }
    QList<int> getDrumMapSfIndex(int presetIndex) { return drumSf[presetIndex]; }


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
    int  device(InstrumentType t);
    int  busGroup(InstrumentType t);
    int  volume(InstrumentType t);
    bool isMute(InstrumentType t);
    bool isSolo(InstrumentType t);
    int  useVSTi(InstrumentType t);
    SpeakerType speaker(InstrumentType t);
    void setDevice(InstrumentType t, int device);
    void setBusGroup(InstrumentType t, int group);
    void setVolume(InstrumentType t, int volume);
    void setMute(InstrumentType t, bool m);
    void setSolo(InstrumentType t, bool s);
    void setUseVSTi(InstrumentType t, int vstiIndex);
    void setSpeaker(InstrumentType t, SpeakerType speaker);


    static QStringList audioDevices();
    static void audioDevices(const QMap<int, QString> &devices);
    static bool isSoundFontFile(const QString &sfile);

    // Fx ----------------------
    QList<Equalizer31BandFX *> equalizer31BandFXs();
    QList<Reverb2FX *> reverbFXs();
    QList<Chorus2FX *> chorusFXs();

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
    QList<QByteArray> fxChunks(InstrumentType type);
    QList<QList<float>> fxParams(InstrumentType type);

    #ifndef __linux__
    BASS_VST_INFO vstiInfo(int vstiIndex);
    QStringList vstiFiles();
    QString vstiFile(int vstiIndex);
    DWORD vstiHandle(int vstiIndex);
    int vstiProgram(int vstiIndex);
    QList<float> vstiParams(int vstiIndex);
    QByteArray vstiChunk(int vstiIndex);
    DWORD setVSTiFile(int vstiIndex, const QString &file);
    void removeVSTiFile(int vstiIndex);
    #endif

    const int HANDLE_STREAM_COUNT = 62;
    const int HANDLE_MIDI_COUNT = 46;
    const int HANDLE_VSTI_START = 42;
    const int HANDLE_VSTI_COUNT = 4;
    const int HANDLE_BUS_COUNT = 16;
    const int HANDLE_BUS_START = 46;

signals:
    void noteOnSended(InstrumentType t, int bus, int ch, int note, int velocity);

private:
    DWORD createStream(InstrumentType t);

    void sendToAllMidiStream(int ch, DWORD eventType, DWORD param);
    void setSfToStream();
    void calculateEnable();
    HSTREAM getDrumHandleFromNote(int drumNote);

private:
    QList<MixerHandle> mixers;
    //MixerManager mixers;
    //HSTREAM mixHandle;
    QMap<InstrumentType, HSTREAM> handles;
    QList<HSOUNDFONT> synth_HSOUNDFONT;
    int sfPreset = 0;
    QStringList sfFiles;
    QList<QList<int>> instmSf;
    QList<QList<int>> drumSf;
    QMap<InstrumentType, Instrument> instMap;
    InstrumentType chInstType[16];

    #ifndef __linux__
    QString mVstiFiles[4];
    BASS_VST_INFO mVstiInfos[4];
    int mVstiTempProgram[4];
    QList<float> mVstiTempParams[4];
    QByteArray mVstiChunk[4];
    #endif

    // unique ID,  VSTNamePAth
    QMap<uint, VSTNamePath> _vstList;
    // ---------------------------

    float synth_volume = 1.0f;
    bool openned = false;
    bool useSolo = false;

    int defaultDev = 1;
    bool useFloat = true;
    bool useFX = false;
    bool sfLoadAll = false;

    DWORD RPNType = 0;

    // device number, name
    static QMap<int, QString> outDevices;
};

#endif // MIDISYNTHESIZER_H
