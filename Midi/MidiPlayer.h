#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include "MidiOut.h"
#include "Channel.h"
#include "MidiSequencer.h"
#include "MidiSynthesizer.h"

#include <QObject>

enum class PlayerState
{
    Playing,
    Stopped,
    Paused,
    Finished
};

class MidiPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MidiPlayer(QObject *parent = nullptr);
    ~MidiPlayer();

    static QStringList midiDevices();
    static QStringList midiInDevices();
    static bool isSnareNumber(int num);
    static bool isBassInstrument(int ints);

    MidiSequencer *midiSequencer() { return _midiSeq; }
    MidiSynthesizer *midiSynthesizer() { return _midiSynth; }
    Channel *midiChannel() { return _midiChannels; }
    int midiOutPortNumber() { return _midiPortNum; }
    int midiInPortNumber() { return _midiPortInNum; }
    int volume() { return _volume; }
    int transpose() { return _midiTranspose; }

    int  lockDrumNumber()  { return _lockDrumNumber; }
    int  lockSnareNumber() { return _lockSnareNumber; }
    int  lockBassNumber()  { return _lockBassBumber; }
    bool isLockDrum()   { return _lockDrum; }
    bool isLockSnare()  { return _lockSnare; }
    bool isLockBass()   { return _lockBass; }

    MidiFile *midiFile();
    MidiFile *midiFileTemp();

    bool isUsedMidiSynthesizer();

    bool isPlayerPlaying();
    bool isPlayerStopped();
    bool isPlayerPaused();
    bool isPlayerFinished();
    PlayerState playerState();

    long durationMs();
    long positionMs();
    int durationTick();
    int positionTick();
    int bpmSpeed();
    int currentBpm();
    int currentBeat();
    int beatCount();

    bool setMidiOut(int portNumber);
    bool setMidiIn(int portNumber);
    bool load(const QString &file, bool seekFileChunkID = false);
    void play();
    void stop(bool resetPos = false);
    void setVolume(int v);
    void setVolume(int ch, int v);
    void setInstrument(int ch, int i);
    void setMute(int ch, bool mute);
    void setSolo(int ch, bool solo);
    void setPan(int ch, int v);
    void setReverb(int ch, int v);
    void setChorus(int ch, int v);
    void setLockVolume(int ch, bool lock);

    void setPositionTick(int t);
    void setTranspose(int t);
    void setBpmSpeed(int sp);

    void setLockDrum(bool lock, int number = 0);
    void setLockSnare(bool lock, int number = 38);
    void setLockBass(bool lock, int number = 32);

    void setMapChannelOutput(int ch, int port);
    void receiveMidiIn(std::vector< unsigned char > *message);

    bool isUseMedley() { return _useMedley; }
    void setUseMedley(bool use);

    int medleyBPM() { return _medleyBPM; }
    void setMedleyBPM(int bpm);

    bool loadNextMedley(const QString &file, int startBar, int endBar, int midiSpeed, int transpose);

public slots:
    void sendEvent(MidiEvent *e);

signals:
    void loaded();
    void finished();
    void sendedEvent(MidiEvent *e);
    void bpmChanged(int bpm);
    void nextMedleyStarted();

private slots:
    void onSeqFinished();
    void onSeqBpmChanged(int bpm);

private:
    void sendEventToDevices(MidiEvent *e);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
    void sendAllSoundOff(int ch);
    void sendAllSoundOff();
    void sendResetAllControllers(int ch);
    void sendResetAllControllers();

    int getNoteNumberToPlay(int ch, int defaultNote);
    void calculateUsedPort();

private:
    MidiSequencer *_midiSeq;
    MidiSequencer *_midiSeqTemp = nullptr;
    QMap<int, MidiOut*> _midiOuts;
    MidiSynthesizer     *_midiSynth;
    RtMidiIn            *_midiIn = nullptr;
    Channel             _midiChannels[16];
    int                 _midiPortNum = 0;
    int                 _midiPortInNum = -1;
    int                 _volume = 100;
    int                 _midiTranspose = 0;
    int                 _midiTransposeTemp = 0;
    int                 _seqIndex = 0;
    int                 _medleyBPM = 120;
    bool                _useMedley = false;
    bool                _useSolo = false;

    QString _medleyId = "";

    MidiEvent   _tempEvent, _midiInEvent;
    MidiEvent*  _playingEventPtr = nullptr;

    bool    _lockDrum  = false;
    bool    _lockSnare = false;
    bool    _lockBass  = false;
    int     _lockDrumNumber  = 0;
    int     _lockSnareNumber = 38;
    int     _lockBassBumber  = 32;
};

void midiIncallback( double deltatime, std::vector< unsigned char > *message, void *userData );

#endif // MIDIPLAYER_H
