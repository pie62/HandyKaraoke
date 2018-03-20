#ifndef MIDISEQUENCER_H
#define MIDISEQUENCER_H

#include <QThread>
#include <QElapsedTimer>

#include "MidiFile.h"


class MidiSequencer : public QThread
{
    Q_OBJECT
public:
    explicit MidiSequencer(QObject *parent = nullptr);
    ~MidiSequencer();

    MidiFile* midiFile() { return _midi; }

    bool isSeqFinished() { return _finished; }
    bool isSeqPlaying() { return _playing; }
    bool isSeqStopped() { return _stopped; }
    bool isSeqPaused() { return (!_playing && !_stopped) ? true : false; }

    int bpmSpeed() { return _midiSpeed; }
    int currentBpm() { return _midiBpm + _midiSpeed; }

    int beatCount();
    int currentBeat();
    int positionTick();
    int durationTick();
    long positionMs();
    long durationMs();

    void setPositionTick(int t);
    void setBpmSpeed(int sp);


    bool load(const QString &file, bool seekFileChunkID = false);
    void stop(bool resetPos = false);

public slots:

signals:
    void playingEvent(MidiEvent *e);

protected:
    void run();

private:
    MidiFile *_midi;
    QElapsedTimer *_eTimer;

    int     _midiBpm = 120;
    int     _midiSpeed = 0;
    int     _midiSpeedTemp = 0;
    bool    _midiChangeBpmSpeed = false;

    int     _positionTick = 0;
    long    _positionMs = 0;

    int     _playedIndex = 0;
    long    _startPlayTime = 0;
    long    _startPlayIndex = 0;
    bool    _finished = false;
    bool    _stopped = true;
    bool    _playing = false;
};

#endif // MIDISEQUENCER_H
