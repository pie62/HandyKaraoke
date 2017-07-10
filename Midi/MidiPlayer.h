#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include "MidiFile.h"
#include "MidiOut.h"
#include "Channel.h"
#include "MidiSynthesizer.h"

#include <QThread>
#include <QElapsedTimer>
#include <QMap>

class MidiPlayer : public QThread
{
    Q_OBJECT
public:
    explicit MidiPlayer(QObject *parent = 0);
    ~MidiPlayer();

    static std::vector<std::string> midiDevices();
    bool setMidiOut(int portNumer);
    bool load(std::string file);
    void stop(bool resetPos = false);
    void setVolume(int v);
    void setVolume(int ch, int v);
    void setInstrument(int ch, int i);
    void setMute(int ch, bool mute);
    void setSolo(int ch, bool solo);
    void setPan(int ch, int v);
    void setReverb(int ch, int v);
    void setChorus(int ch, int v);
    void setPositionTick(int t);

    MidiSynthesizer* midiSynthesizer() { return _midiSynth; }
    MidiFile* midiFile() { return _midi; }
    Channel* midiChannel() { return _midiChannels; }
    int midiOutPortNumber() { return _midiPortNum; }
    int volume() { return _volume; }
    int durationTick() { return _durationTick; }
    long durationMs() { return _durationMs; }
    bool isPlayerFinished() { return _finished; }
    bool isPlayerPlaying() { return _playing; }
    bool isPlayerStopped() { return _stopped; }
    bool isPlayerPaused() { return (!_playing && !_stopped) ? true : false; }
    long positionMs();
    int positionTick();
    int currentBeat();

    int currentBpm() { return _midiBpm; }
    int beatCount() { return _midiBeatCount; }
    QMap<int, int> beatInBar() { return _beatInBar; }

    static bool isSnareNumber(int num);
    static bool isBassInstrument(int ints);

    int  lockDrumNumber()  { return _lockDrumNumber; }
    int  lockSnareNumber() { return _lockSnareNumber; }
    int  lockBassNumber()  { return _lockBassBumber; }

    bool isLockDrum()   { return _lockDrum; }
    bool isLockSnare()  { return _lockSnare; }
    bool isLockBass()   { return _lockBass; }

    void setLockDrum(bool lock, int number = 0);
    void setLockSnare(bool lock, int number = 38);
    void setLockBass(bool lock, int number = 32);

    static int getNumberBeatInBar(int numerator, int denominator);

protected:
    void run();

signals:
    void loaded();
    void playingEvents(MidiEvent *e);
    void bpmChanged(int bpm);

private:
    MidiFile            *_midi;
    MidiOut             *_midiOut;
    MidiSynthesizer     *_midiSynth;
    Channel             _midiChannels[16];
    int                 _midiPortNum = 0;
    int                 _midiBpm = 120;
    int                 _midiSpeed = 0;
    int                 _midiTranspose = 0;
    int                 _midiBeatCount = 0;

    int     _volume = 100;
    int     _durationTick = 0;
    int     _positionTick = 0;
    long    _durationMs = 0;
    long    _positionMs = 0;

    int     _playedIndex = 0;
    long    _startPlayTime = 0;
    bool    _finished = false;
    bool    _stopped = true;
    bool    _playing = false;
    bool    _useSolo = false;

    bool    _lockDrum  = false;
    bool    _lockSnare = false;
    bool    _lockBass  = false;
    int     _lockDrumNumber  = 0;
    int     _lockSnareNumber = 38;
    int     _lockBassBumber  = 32;

    // number beat in 1 bar , number bar
    QMap<int, int> _beatInBar;
    QElapsedTimer *_eTimer;

    void playEvents();
    void sendEvent(MidiEvent *e);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
    void sendResetAllControllers();

    int getNoteNumberToPlay(int ch, int defaultNote);
};

#endif // MIDIPLAYER_H
