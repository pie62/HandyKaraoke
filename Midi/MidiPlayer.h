#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include "MidiFile.h"
#include "MidiOut.h"
#include "Channel.h"
#include "MidiSynthesizer.h"

#include <QThread>
#include <QElapsedTimer>

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
    void setPositionTick(int t);

    MidiSynthesizer* midiSynthesizer() { return _midiSynth; }
    MidiFile* midiFile() { return _midi; }
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

protected:
    void run();

signals:
    void playingEvents(MidiEvent *e);

private:
    MidiFile            *_midi;
    MidiOut             *_midiOut;
    MidiSynthesizer     *_midiSynth;
    Channel             _midiChannels[16];
    int                 _midiPortNum = 0;
    int                 _midiBpm = 120;
    int                 _midiSpeed = 0;

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

    QElapsedTimer *_eTimer;

    void playEvents();
    void sendEvent(MidiEvent *e);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
};

#endif // MIDIPLAYER_H
